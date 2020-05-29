#### to do : include header files with definitions ####

/* This routine converts the unsigned char data to float.  In the */
/* process it shifts and scales the data so the values range from */
/* +/- 128.0 This function returns on error.                      */

int conv_img_2_flt_ret(
   float *fip,         /* output float image data  */
   float *m_shift,     /* shifting parameter       */
   float *r_scale,     /* scaling parameter        */
   unsigned char *data,        /* input unsigned char data */
   const int num_pix)  /* num pixels in image      */

{
   int cnt;                     /* pixel cnt */
   unsigned int sum, overflow;  /* sum of pixel values */
   float mean;                  /* mean pixel value */
   int low, high;               /* low/high pixel values */
   float low_diff, high_diff;   /* new low/high pixels values shifting */

   sum = 0;
   overflow = 0;
   low = 255;
   high = 0;
   for(cnt = 0; cnt < num_pix; cnt++) {
      if(data[cnt] > high)
         high = data[cnt];
      if(data[cnt] < low)
         low = data[cnt];
      sum += data[cnt];
      if(sum < overflow) {
         fprintf(stderr, "ERROR: conv_img_2_flt: overflow at %d\n", cnt);
         return(-91);
      }
      overflow = sum;
   }

   mean = (float) sum / (float)num_pix;
   *m_shift = mean;

   low_diff = *m_shift - low;
   high_diff = high - *m_shift;

   if(low_diff >= high_diff)
      *r_scale = low_diff;
   else
      *r_scale = high_diff;

   *r_scale /= (float)128.0;

   for(cnt = 0; cnt < num_pix; cnt++) {
      fip[cnt] = ((float)data[cnt] - *m_shift) / *r_scale;
   }
   return(0);
}

/* This routine calculates the variances of the subbands. */

void variance(
   QUANT_VALS *quant_vals, /* quantization parameters */
   Q_TREE q_tree[],        /* quantization "tree"     */
   const int q_treelen,    /* length of q_tree        */
   float *fip,             /* image pointer           */
   const int width,        /* image width             */
   const int height)       /* image height            */
{
   float *fp;              /* temp image pointer */
   int cvr;                /* subband counter */
   int lenx = 0, leny = 0; /* dimensions of area to calculate variance */
   int skipx, skipy;       /* pixels to skip to get to area for
                              variance calculation */
   int row, col;           /* dimension counters */
   float ssq;             /* sum of squares */
   float sum2;            /* variance calculation parameter */
   float sum_pix;         /* sum of pixels */
   float vsum;            /* variance sum for subbands 0-3 */


   vsum = 0.0;
   for(cvr = 0; cvr < 4; cvr++) {
      fp = fip + (q_tree[cvr].y * width) + q_tree[cvr].x;
      ssq = 0.0;
      sum_pix = 0.0;

      skipx = q_tree[cvr].lenx / 8;
      skipy = (9 * q_tree[cvr].leny)/32;

      lenx = (3 * q_tree[cvr].lenx)/4;
      leny = (7 * q_tree[cvr].leny)/16;

      fp += (skipy * width) + skipx;
      for(row = 0; row < leny; row++, fp += (width - lenx)) {
         for(col = 0; col < lenx; col++) {
            sum_pix += *fp;
            ssq += *fp * *fp;
            fp++;
         }
      }
      sum2 = (sum_pix * sum_pix)/(lenx * leny);
      quant_vals->var[cvr] = (float)((ssq - sum2)/((lenx * leny)-1.0));
      vsum += quant_vals->var[cvr];
   }

   if(vsum < 20000.0) {
      for(cvr = 0; cvr < NUM_SUBBANDS; cvr++) {
         fp = fip + (q_tree[cvr].y * width) + q_tree[cvr].x;
         ssq = 0.0;
         sum_pix = 0.0;

         lenx = q_tree[cvr].lenx;
         leny = q_tree[cvr].leny;

         for(row = 0; row < leny; row++, fp += (width - lenx)) {
            for(col = 0; col < lenx; col++) {
               sum_pix += *fp;
               ssq += *fp * *fp;
               fp++;
            }
         }
         sum2 = (sum_pix * sum_pix)/(lenx * leny);
         quant_vals->var[cvr] = (float)((ssq - sum2)/((lenx * leny)-1.0));
      }
   }
   else {
      for(cvr = 4; cvr < NUM_SUBBANDS; cvr++) {
         fp = fip + (q_tree[cvr].y * width) + q_tree[cvr].x;
         ssq = 0.0;
         sum_pix = 0.0;

         skipx = q_tree[cvr].lenx / 8;
         skipy = (9 * q_tree[cvr].leny)/32;

         lenx = (3 * q_tree[cvr].lenx)/4;
         leny = (7 * q_tree[cvr].leny)/16;

         fp += (skipy * width) + skipx;
         for(row = 0; row < leny; row++, fp += (width - lenx)) {
            for(col = 0; col < lenx; col++) {
               sum_pix += *fp;
               ssq += *fp * *fp;
               fp++;
            }
         }
         sum2 = (sum_pix * sum_pix)/(lenx * leny);
         quant_vals->var[cvr] = (float)((ssq - sum2)/((lenx * leny)-1.0));
      }
   }
}


/************************************************/
/* This routine quantizes the wavelet subbands. */
/************************************************/
int quantize(
   short **osip,           /* quantized output             */
   int *ocmp_siz,          /* size of quantized output     */
   QUANT_VALS *quant_vals, /* quantization parameters      */
   Q_TREE q_tree[],        /* quantization "tree"          */
   const int q_treelen,    /* size of q_tree               */
   float *fip,             /* floating point image pointer */
   const int width,        /* image width                  */
   const int height)       /* image height                 */
{
   int i;                 /* temp counter */
   int j;                 /* interation index */
   float *fptr;           /* temp image pointer */
   short *sip, *sptr;     /* pointers to quantized image */
   int row, col;          /* temp image characteristic parameters */
   int cnt;               /* subband counter */
   float zbin;            /* zero bin size */
   float A[NUM_SUBBANDS]; /* subband "weights" for quantization */
   float m[NUM_SUBBANDS]; /* subband size to image size ratios */
                          /* (reciprocal of FBI spec for 'm')  */
   float m1, m2, m3;      /* reciprocal constants for 'm' */
   float sigma[NUM_SUBBANDS]; /* square root of subband variances */
   int K0[NUM_SUBBANDS];  /* initial list of subbands w/variance >= thresh */
   int K1[NUM_SUBBANDS];  /* working list of subbands */
   int *K, *nK;           /* pointers to sets of subbands */
   int NP[NUM_SUBBANDS];  /* current subbounds with nonpositive bit rates. */
   int K0len;             /* number of subbands in K0 */
   int Klen, nKlen;       /* number of subbands in other subband lists */
   int NPlen;             /* number of subbands flagged in NP */
   float S;               /* current frac of subbands w/positive bit rate */
   float q;               /* current proportionality constant */
   float P;               /* product of 'q/Q' ratios */

   /* Set up 'A' table. */
   for(cnt = 0; cnt < STRT_SUBBAND_3; cnt++)
      A[cnt] = 1.0;
   A[cnt++ /*52*/] = 1.32;
   A[cnt++ /*53*/] = 1.08;
   A[cnt++ /*54*/] = 1.42;
   A[cnt++ /*55*/] = 1.08;
   A[cnt++ /*56*/] = 1.32;
   A[cnt++ /*57*/] = 1.42;
   A[cnt++ /*58*/] = 1.08;
   A[cnt++ /*59*/] = 1.08;

   for(cnt = 0; cnt < MAX_SUBBANDS; cnt++) {
      quant_vals->qbss[cnt] = 0.0;
      quant_vals->qzbs[cnt] = 0.0;
   }

   /* Set up 'Q1' (prime) table. */
   for(cnt = 0; cnt < NUM_SUBBANDS; cnt++) {
      if(quant_vals->var[cnt] < VARIANCE_THRESH)
         quant_vals->qbss[cnt] = 0.0;
      else
         /* NOTE: q has been taken out of the denominator in the next */
         /*       2 formulas from the original code. */
         if(cnt < STRT_SIZE_REGION_2 /*4*/)
            quant_vals->qbss[cnt] = 1.0;
         else
            quant_vals->qbss[cnt] = 10.0 / (A[cnt] *
                                    (float)log(quant_vals->var[cnt]));
   }


   /* Set up output buffer. */
   if((sip = (short *) calloc(width*height, sizeof(short))) == NULL) {
      fprintf(stderr,"ERROR : quantize : calloc : sip\n");
      return(-90);
   }
   sptr = sip;

   /* Set up 'm' table (these values are the reciprocal of 'm' in */
   /* the FBI spec).                                              */
   m1 = 1.0/1024.0;
   m2 = 1.0/256.0;
   m3 = 1.0/16.0;
   for(cnt = 0; cnt < STRT_SIZE_REGION_2; cnt++)
      m[cnt] = m1;
   for(cnt = STRT_SIZE_REGION_2; cnt < STRT_SIZE_REGION_3; cnt++)
      m[cnt] = m2;
   for(cnt = STRT_SIZE_REGION_3; cnt < NUM_SUBBANDS; cnt++)
      m[cnt] = m3;

   j = 0;
   /* Initialize 'K0' and 'K1' lists. */
   K0len = 0;
   for(cnt = 0; cnt < NUM_SUBBANDS; cnt++){
      if(quant_vals->var[cnt] >= VARIANCE_THRESH){
         K0[K0len] = cnt;
         K1[K0len++] = cnt;
         /* Compute square root of subband variance. */
         sigma[cnt] = sqrt(quant_vals->var[cnt]);
      }
   }
   K = K1;
   Klen = K0len;

   while(1){
      /* Compute new 'S' */
      S = 0.0;
      for(i = 0; i < Klen; i++){
         /* Remeber 'm' is the reciprocal of spec. */
         S += m[K[i]];
      }

      /* Compute product 'P' */
      P = 1.0;
      for(i = 0; i < Klen; i++){
         /* Remeber 'm' is the reciprocal of spec. */
         P *= pow((sigma[K[i]] / quant_vals->qbss[K[i]]), m[K[i]]);
      }

      /* Compute new 'q' */
      q = (pow(2,((quant_vals->r/S)-1.0))/2.5) / pow(P, (1.0/S));

      /* Flag subbands with non-positive bitrate. */
      memset(NP, 0, NUM_SUBBANDS * sizeof(int));
      NPlen = 0;
      for(i = 0; i < Klen; i++){
         if((quant_vals->qbss[K[i]] / q) >= (5.0*sigma[K[i]])){
            NP[K[i]] = TRUE;
            NPlen++;
         }
      }

      /* If list of subbands with non-positive bitrate is empty ... */
      if(NPlen == 0){
         /* Then we are done, so break from while loop. */
         break;
      }

      /* Assign new subband set to previous set K minus subbands in set NP. */
      nK = K1;
      nKlen = 0;
      for(i = 0; i < Klen; i++){
         if(!NP[K[i]])
            nK[nKlen++] = K[i];
      }

      /* Assign new set as K. */
      K = nK;
      Klen = nKlen;

      /* Bump iteration counter. */
      j++;
   }

   /* Flag subbands that are in set 'K0' (the very first set). */
   nK = K1;
   memset(nK, 0, NUM_SUBBANDS * sizeof(int));
   for(i = 0; i < K0len; i++){
      nK[K0[i]] = TRUE;
   }
   /* Set 'Q' values. */
   for(cnt = 0; cnt < NUM_SUBBANDS; cnt++) {
      if(nK[cnt])
         quant_vals->qbss[cnt] /= q;
      else
         quant_vals->qbss[cnt] = 0.0;
      quant_vals->qzbs[cnt] = 1.2 * quant_vals->qbss[cnt];
   }

   /* Now ready to compute and store bin widths for subbands. */
   for(cnt = 0; cnt < NUM_SUBBANDS; cnt++) {
      fptr = fip + (q_tree[cnt].y * width) + q_tree[cnt].x;

      if(quant_vals->qbss[cnt] != 0.0) {

         zbin = quant_vals->qzbs[cnt] / 2.0;

         for(row = 0;
            row < q_tree[cnt].leny;
            row++, fptr += width - q_tree[cnt].lenx){
            for(col = 0; col < q_tree[cnt].lenx; col++) {
               if(-zbin <= *fptr && *fptr <= zbin)
                  *sptr = 0;
               else if(*fptr > 0.0)
                  *sptr = (short)(((*fptr-zbin)/quant_vals->qbss[cnt]) + 1.0);
               else
                  *sptr = (short)(((*fptr+zbin)/quant_vals->qbss[cnt]) - 1.0);
               sptr++;
               fptr++;
            }
         }
      }
      else if(debug > 0)
         fprintf(stderr, "%d -> %3.6f\n", cnt, quant_vals->qbss[cnt]);
   }

   *osip = sip;
   *ocmp_siz = sptr - sip;
   return(0);
}


/************************************************************************/
/* Compute quantized WSQ subband block sizes.                           */
/************************************************************************/
void quant_block_sizes(int *oqsize1, int *oqsize2, int *oqsize3,
                 QUANT_VALS *quant_vals,
                 W_TREE w_tree[], const int w_treelen,
                 Q_TREE q_tree[], const int q_treelen)
{
   int qsize1, qsize2, qsize3;
   int node;

   /* Compute temporary sizes of 3 WSQ subband blocks. */
   qsize1 = w_tree[14].lenx * w_tree[14].leny;
   qsize2 = (w_tree[5].leny * w_tree[1].lenx) +
            (w_tree[4].lenx * w_tree[4].leny);
   qsize3 = (w_tree[2].lenx * w_tree[2].leny) +
            (w_tree[3].lenx * w_tree[3].leny);

   /* Adjust size of quantized WSQ subband blocks. */
   for (node = 0; node < STRT_SUBBAND_2; node++)
      if(quant_vals->qbss[node] == 0.0)
         qsize1 -= (q_tree[node].lenx * q_tree[node].leny);

   for (node = STRT_SUBBAND_2; node < STRT_SUBBAND_3; node++)
      if(quant_vals->qbss[node] == 0.0)
          qsize2 -= (q_tree[node].lenx * q_tree[node].leny);

   for (node = STRT_SUBBAND_3; node < STRT_SUBBAND_DEL; node++)
      if(quant_vals->qbss[node] == 0.0)
         qsize3 -= (q_tree[node].lenx * q_tree[node].leny);

   *oqsize1 = qsize1;
   *oqsize2 = qsize2;
   *oqsize3 = qsize3;
}

/************************************************************************/
/* WSQ decompose the image.  NOTE: this routine modifies and returns    */
/* the results in "fdata".                                              */
/************************************************************************/
int wsq_decompose(float *fdata, const int width, const int height,
                  W_TREE w_tree[], const int w_treelen,
                  float *hifilt, const int hisz,
                  float *lofilt, const int losz)
{
   int num_pix, node;
   float *fdata1, *fdata_bse;

   num_pix = width * height;
   /* Allocate temporary floating point pixmap. */
   if((fdata1 = (float *) malloc(num_pix*sizeof(float))) == NULL) {
      fprintf(stderr,"ERROR : wsq_decompose : malloc : fdata1\n");
      return(-94);
   }

   /* Compute the Wavelet image decomposition. */
   for(node = 0; node < w_treelen; node++) {
      fdata_bse = fdata + (w_tree[node].y * width) + w_tree[node].x;
      get_lets(fdata1, fdata_bse, w_tree[node].leny, w_tree[node].lenx,
               width, 1, hifilt, hisz, lofilt, losz, w_tree[node].inv_rw);
      get_lets(fdata_bse, fdata1, w_tree[node].lenx, w_tree[node].leny,
               1, width, hifilt, hisz, lofilt, losz, w_tree[node].inv_cl);
   }
   free(fdata1);

   return(0);
}
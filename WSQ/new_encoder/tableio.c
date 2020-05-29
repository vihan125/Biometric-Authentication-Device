
/************************************************/
/* Stores transform table to the output buffer. */
/************************************************/


int putc_transform_table(
   float *lofilt,     /* filter coefficients      */
   const int losz,
   float *hifilt,
   const int hisz,
   unsigned char *odata,      /* output byte buffer       */
   const int oalloc,  /* allocated size of buffer */
   int   *olen)       /* filled length of buffer  */
{
   int ret;
   unsigned int coef;           /* filter coefficient indicator */
   unsigned int int_dat;        /* temp variable */
   float dbl_tmp;       /* temp variable */
   char scale_ex, sign; /* exponent scaling and sign parameters */

   if(debug > 0)
      fprintf(stderr, "Writing transform table.\n");

   if((ret = putc_ushort(DTT_WSQ, odata, oalloc, olen)))
      return(ret);
   /* table size */
   if((ret = putc_ushort(58, odata, oalloc, olen)))
      return(ret);
   /* number analysis lowpass coefficients */
   if((ret = putc_byte(losz, odata, oalloc, olen)))
      return(ret);
   /* number analysis highpass coefficients */
   if((ret = putc_byte(hisz, odata, oalloc, olen)))
      return(ret);

   for(coef = (losz>>1); coef < losz; coef++) {
      dbl_tmp = lofilt[coef];
      if(dbl_tmp >= 0.0) {
         sign = 0;
      }
      else {
         sign = 1;
         dbl_tmp *= -1.0;
      }
      scale_ex = 0;
      if(dbl_tmp == 0.0)
         int_dat = 0;
      else if(dbl_tmp < 4294967295.0) {
         while(dbl_tmp < 4294967295.0) {
            scale_ex += 1;
            dbl_tmp *= 10.0;
         }
         scale_ex -= 1;
         int_dat = (unsigned int)sround_uint(dbl_tmp / 10.0);
      }
      else {
         dbl_tmp = lofilt[coef];
         fprintf(stderr,
         "ERROR: putc_transform_table : lofilt[%d] to high at %f\n",
         coef, dbl_tmp);
         return(-82);
      }

      if(debug > 2) {
         fprintf(stderr, "lo[%d] = %u\n", coef, int_dat);
         fprintf(stderr, "lof[%d] = %0.15f\n", coef, lofilt[coef]);
      }

      if((ret = putc_byte(sign, odata, oalloc, olen)))
         return(ret);
      if((ret = putc_byte(scale_ex, odata, oalloc, olen)))
         return(ret);
      if((ret = putc_uint(int_dat, odata, oalloc, olen)))
         return(ret);
   }

   for(coef = (hisz>>1); coef < hisz; coef++) {
      dbl_tmp = hifilt[coef];
      if(dbl_tmp >= 0.0) {
         sign = 0;
      }
      else {
         sign = 1;
         dbl_tmp *= -1.0;
      }
      scale_ex = 0;
      if(dbl_tmp == 0.0)
         int_dat = 0;
      else if(dbl_tmp < 4294967295.0) {
         while(dbl_tmp < 4294967295.0) {
            scale_ex += 1;
            dbl_tmp *= 10.0;
         }
         scale_ex -= 1;
         int_dat = (unsigned int)sround_uint(dbl_tmp / 10.0);
      }
      else {
         dbl_tmp = hifilt[coef];
         fprintf(stderr,
         "ERROR: putc_transform_table : hifilt[%d] to high at %f\n",
         coef, dbl_tmp);
         return(-83);
      }

      if(debug > 2) {
         fprintf(stderr, "hi[%d] = %u\n", coef, int_dat);
         fprintf(stderr, "hif[%d] = %0.15f\n", coef, hifilt[coef]);
      }

      if((ret = putc_byte(sign, odata, oalloc, olen)))
         return(ret);
      if((ret = putc_byte(scale_ex, odata, oalloc, olen)))
         return(ret);
      if((ret = putc_uint(int_dat, odata, oalloc, olen)))
         return(ret);
   }

   if(debug > 0)
      fprintf(stderr, "Finished writing transform table.\n\n");

   return(0);
}


/***************************************************/
/* Stores quantization table in the output buffer. */
/***************************************************/




int putc_quantization_table(
   QUANT_VALS *quant_vals,   /* quantization parameters  */
   unsigned char *odata,             /* output byte buffer       */
   const int oalloc,         /* allocated size of buffer */
   int   *olen)              /* filled length of buffer  */
{
   int ret, sub;               /* subband indicators */
   char scale_ex, scale_ex2;   /* exponent scaling parameters */
   unsigned short shrt_dat, shrt_dat2; /* temp variables */
   float flt_tmp;              /* temp variable */


   if(debug > 0)
      fprintf(stderr, "Writing quantization table.\n");

   if((ret = putc_ushort(DQT_WSQ, odata, oalloc, olen)))
      return(ret);
   /* table size */
   if((ret = putc_ushort(389, odata, oalloc, olen)))
      return(ret);
   /* exponent scaling value */
   if((ret = putc_byte(2, odata, oalloc, olen)))
      return(ret);
   /* quantizer bin center parameter */
   if((ret = putc_ushort(44, odata, oalloc, olen)))
      return(ret);

   for(sub = 0; sub < 64; sub ++) {
      if(sub >= 0 && sub < 60) {
         if(quant_vals->qbss[sub] != 0.0) {
            flt_tmp = quant_vals->qbss[sub];
            scale_ex = 0;
            if(flt_tmp < 65535) {
               while(flt_tmp < 65535) {
                  scale_ex += 1;
                  flt_tmp *= 10;
               }
               scale_ex -= 1;
               shrt_dat = (unsigned short)sround(flt_tmp / 10.0);
            }
            else {
               flt_tmp = quant_vals->qbss[sub];
               fprintf(stderr,
               "ERROR : putc_quantization_table : Q[%d] to high at %f\n",
               sub, flt_tmp);
               return(-86);
            }

            flt_tmp = quant_vals->qzbs[sub];
            scale_ex2 = 0;
            if(flt_tmp < 65535) {
               while(flt_tmp < 65535) {
                  scale_ex2 += 1;
                  flt_tmp *= 10;
               }
               scale_ex2 -= 1;
               shrt_dat2 = (unsigned short)sround(flt_tmp / 10.0);
            }
            else {
               flt_tmp = quant_vals->qzbs[sub];
               fprintf(stderr,
               "ERROR : putc_quantization_table : Z[%d] to high at %f\n",
               sub, flt_tmp);
               return(-87);
            }
         }
         else {
            scale_ex = 0;
            scale_ex2 = 0;
            shrt_dat = 0;
            shrt_dat2 = 0;
         }
      }
      else {
         scale_ex = 0;
         scale_ex2 = 0;
         shrt_dat = 0;
         shrt_dat2 = 0;
      }

      if(debug > 2) {
         fprintf(stderr,
         "qi[%d] = %d    ::  zi[%d] = %d\n", sub, shrt_dat, sub, shrt_dat2);
         fprintf(stderr,
         "q[%d] = %5.7f  ::  z[%d] = %5.7f\n", sub, quant_vals->qbss[sub],
          sub, quant_vals->qzbs[sub]);
      }

      if((ret = putc_byte(scale_ex, odata, oalloc, olen)))
         return(ret);
      if((ret = putc_ushort(shrt_dat, odata, oalloc, olen)))
         return(ret);
      if((ret = putc_byte(scale_ex2, odata, oalloc, olen)))
         return(ret);
      if((ret = putc_ushort(shrt_dat2, odata, oalloc, olen)))
         return(ret);
   }

   if(debug > 0)
      fprintf(stderr, "Finished writing quantization table.\n\n");

   return(0);
}




/*********************************************/
/* Stores frame header to the output buffer. */
/*********************************************/
int putc_frame_header_wsq(
   const int width,       /* image width              */
   const int height,      /* image height             */
   const float m_shift,   /* image shifting parameter */
   const float r_scale,   /* image scaling parameter  */
   unsigned char *odata,          /* output byte buffer       */
   const int oalloc,      /* allocated size of buffer */
   int   *olen)           /* filled length of buffer  */
{
   int ret;
   float flt_tmp;         /* temp variable */
   char scale_ex;         /* exponent scaling parameter */
   unsigned short shrt_dat;       /* temp variable */

   if(debug > 0)
      fprintf(stderr, "Writing frame header.\n");

   if((ret = putc_ushort(SOF_WSQ, odata, oalloc, olen)))
      return(ret);
   /* size of frame header */
   if((ret = putc_ushort(17, odata, oalloc, olen)))
      return(ret);
   /* black pixel */
   if((ret = putc_byte(0, odata, oalloc, olen)))
      return(ret);
   /* white pixel */
   if((ret = putc_byte(255, odata, oalloc, olen)))
      return(ret);
   if((ret = putc_ushort(height, odata, oalloc, olen)))
      return(ret);
   if((ret = putc_ushort(width, odata, oalloc, olen)))
      return(ret);

   if(debug > 2)
      fprintf(stderr,
              "m_shift = %f  :: r_scale = %f\n", m_shift, r_scale);

   flt_tmp = m_shift;
   scale_ex = 0;
   if(flt_tmp != 0.0) {
      while(flt_tmp < 65535) {
         scale_ex += 1;
         flt_tmp *= 10;
      }
      scale_ex -= 1;
      shrt_dat = (unsigned short)sround(flt_tmp / 10.0);
   }
   else
      shrt_dat = 0;
   if((ret = putc_byte(scale_ex, odata, oalloc, olen)))
      return(ret);
   if((ret = putc_ushort(shrt_dat, odata, oalloc, olen)))
      return(ret);

   flt_tmp = r_scale;
   scale_ex = 0;
   if(flt_tmp != 0.0) {
      while(flt_tmp < 65535) {
         scale_ex += 1;
         flt_tmp *= 10;
      }
      scale_ex -= 1;
      shrt_dat = (unsigned short)sround(flt_tmp / 10.0);
   }
   else
      shrt_dat = 0;
   if((ret = putc_byte(scale_ex, odata, oalloc, olen)))
      return(ret);
   if((ret = putc_ushort(shrt_dat, odata, oalloc, olen)))
      return(ret);
   if((ret = putc_byte(2, odata, oalloc, olen)))
      return(ret);
   if((ret = putc_ushort(0, odata, oalloc, olen)))
      return(ret);

   if(debug > 0)
      fprintf(stderr, "Finished writing frame header.\n\n");

   return(0);
}



/*********************************************/
/* Stores block header to the output buffer. */
/*********************************************/
int putc_block_header(
   const int table,   /* huffman table indicator  */
   unsigned char *odata,      /* output byte buffer       */
   const int oalloc,  /* allocated size of buffer */
   int   *olen)       /* filled length of buffer  */
{
   int ret;

   if(debug > 0)
      fprintf(stderr, "Writing block header.\n");

   if((ret = putc_ushort(SOB_WSQ, odata, oalloc, olen)))
      return(ret);
   /* block header size */
   if((ret = putc_ushort(3, odata, oalloc, olen)))
      return(ret);
   if((ret = putc_byte((unsigned char)table, odata, oalloc, olen)))
      return(ret);

   if(debug > 0)
      fprintf(stderr, "Finished writing block header.\n\n");

   return(0);
}


/*******************************************/
int putc_nistcom_wsq(char *comment_text, const int w, const int h,
                     const int d, const int ppi, const int lossyflag,
                     const float r_bitrate,
                     unsigned char *odata, const int oalloc, int *olen)
{
   int ret, gencomflag;
   NISTCOM *nistcom;
   char *comstr;

   /* Add Comment(s) here. */
   nistcom = (NISTCOM *)NULL;
   gencomflag = 0;
   if(comment_text != (char *)NULL){
      /* if NISTCOM ... */
      if(strncmp(comment_text, NCM_HEADER, strlen(NCM_HEADER)) == 0){
         if((ret = string2fet(&nistcom, comment_text))){
            return(ret);
         }
      }
      /* If general comment ... */
      else{
         gencomflag = 1;
      }
   }
   /* Otherwise, no comment passed ... */

   /* Combine image attributes to NISTCOM. */
   if((ret = combine_wsq_nistcom(&nistcom, w, h, d, ppi, lossyflag, r_bitrate))){
      if(nistcom != (NISTCOM *)NULL)
         freefet(nistcom);
      return(ret);
   }

   /* Put NISTCOM ... */
   /* NISTCOM to string. */
   if((ret = fet2string(&comstr, nistcom))){
      freefet(nistcom);
      return(ret);
   }
   /* Put NISTCOM comment string. */
   if((ret = putc_comment(COM_WSQ, (unsigned char *)comstr, strlen(comstr),
                        odata, oalloc, olen))){
      freefet(nistcom);
      free(comstr);
      return(ret);
   }
   freefet(nistcom);
   free(comstr);

   /* If general comment exists ... */
   if(gencomflag){
      /* Put general comment to its own segment. */
      if((ret = putc_comment(COM_WSQ, (unsigned char *)comment_text,
                             strlen(comment_text), odata, oalloc, olen)))
         return(ret);
   }

   return(0);
}

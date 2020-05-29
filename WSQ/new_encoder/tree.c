/************************************************************************/
/*              Routines used to generate the "trees" used              */
/*              in creating the wavelet subbands (w_tree)               */
/*              and when quantizing the subbands (q_tree) in            */
/*              the WSQ compression/decompression algorithms.           */
/************************************************************************/
/* Build WSQ decomposition trees.                                       */
/************************************************************************/
void build_wsq_trees(W_TREE w_tree[], const int w_treelen,
                     Q_TREE q_tree[], const int q_treelen,
                     const int width, const int height)
{
   /* Build a W-TREE structure for the image. */
   build_w_tree(w_tree, width, height);
   /* Build a Q-TREE structure for the image. */
   build_q_tree(w_tree, q_tree);
}

/********************************************************************/
/* Routine to obtain subband "x-y locations" for creating wavelets. */
/********************************************************************/
void build_w_tree(
   W_TREE w_tree[],   /* wavelet tree structure */
   const int width,   /* image width            */
   const int height)  /* image height           */
{
   int lenx, lenx2, leny, leny2;  /* starting lengths of sections of
                                     the image being split into subbands */
   int node;

   for(node = 0; node < 20; node++) {
      w_tree[node].inv_rw = 0;
      w_tree[node].inv_cl = 0;
   }
   w_tree[2].inv_rw = 1;
   w_tree[4].inv_rw = 1;
   w_tree[7].inv_rw = 1;
   w_tree[9].inv_rw = 1;
   w_tree[11].inv_rw = 1;
   w_tree[13].inv_rw = 1;
   w_tree[16].inv_rw = 1;
   w_tree[18].inv_rw = 1;
   w_tree[3].inv_cl = 1;
   w_tree[5].inv_cl = 1;
   w_tree[8].inv_cl = 1;
   w_tree[9].inv_cl = 1;
   w_tree[12].inv_cl = 1;
   w_tree[13].inv_cl = 1;
   w_tree[17].inv_cl = 1;
   w_tree[18].inv_cl = 1;

   w_tree4(w_tree, 0, 1, width, height, 0, 0, 1);

   if((w_tree[1].lenx % 2) == 0) {
      lenx = w_tree[1].lenx / 2;
      lenx2 = lenx;
   }
   else {
      lenx = (w_tree[1].lenx + 1) / 2;
      lenx2 = lenx - 1;
   }

   if((w_tree[1].leny % 2) == 0) {
      leny = w_tree[1].leny / 2;
      leny2 = leny;
   }
   else {
      leny = (w_tree[1].leny + 1) / 2;
      leny2 = leny - 1;
   }

   w_tree4(w_tree, 4, 6, lenx2, leny, lenx, 0, 0);
   w_tree4(w_tree, 5, 10, lenx, leny2, 0, leny, 0);
   w_tree4(w_tree, 14, 15, lenx, leny, 0, 0, 0);

   w_tree[19].x = 0;
   w_tree[19].y = 0;
   if((w_tree[15].lenx % 2) == 0)
      w_tree[19].lenx = w_tree[15].lenx / 2;
   else
      w_tree[19].lenx = (w_tree[15].lenx + 1) / 2;

   if((w_tree[15].leny % 2) == 0)
      w_tree[19].leny = w_tree[15].leny / 2;
   else
      w_tree[19].leny = (w_tree[15].leny + 1) / 2;

   if(debug > 1) {
      for(node = 0; node < 20; node++)
         fprintf(stderr,
         "t%d -> x = %d  y = %d : dx = %d  dy = %d : ir = %d  ic = %d\n",
         node, w_tree[node].x, w_tree[node].y,
         w_tree[node].lenx, w_tree[node].leny,
         w_tree[node].inv_rw, w_tree[node].inv_cl);
      fprintf(stderr, "\n\n");
   }

   return;
}


/****************************************************************/
void build_q_tree(
   W_TREE *w_tree,  /* wavelet tree structure */
   Q_TREE *q_tree)   /* quantization tree structure */
{
   int node;

   q_tree16(q_tree,3,w_tree[14].lenx,w_tree[14].leny,
              w_tree[14].x,w_tree[14].y, 0, 0);
   q_tree16(q_tree,19,w_tree[4].lenx,w_tree[4].leny,
              w_tree[4].x,w_tree[4].y, 0, 1);
   q_tree16(q_tree,48,w_tree[0].lenx,w_tree[0].leny,
              w_tree[0].x,w_tree[0].y, 0, 0);
   q_tree16(q_tree,35,w_tree[5].lenx,w_tree[5].leny,
              w_tree[5].x,w_tree[5].y, 1, 0);
   q_tree4(q_tree,0,w_tree[19].lenx,w_tree[19].leny,
             w_tree[19].x,w_tree[19].y);

   if(debug > 1) {
      for(node = 0; node < 60; node++)
         fprintf(stderr, "t%d -> x = %d  y = %d : lx = %d  ly = %d\n",
         node, q_tree[node].x, q_tree[node].y,
         q_tree[node].lenx, q_tree[node].leny);
      fprintf(stderr, "\n\n");
   }
   return;
}


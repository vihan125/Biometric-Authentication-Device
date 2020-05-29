/***********************************************/
/* Stores a byte of data to the output buffer. */
/***********************************************/
int putc_byte(
   const unsigned char idata, /* input byte */
   unsigned char *odata,      /* output buffer of bytes */
   const int oalloc,          /* allocated size of output buffer */
   int *olen)                 /* filled length of output buffer  */
{
   /* olen points to next position in odata */
   /* If output buffer is out of space ...  */
   if((*olen) >= oalloc){
      fprintf(stderr,
      "ERROR : putc_byte : buffer overlow : alloc = %d, request = %d\n",
      oalloc, *olen);
      return(-32);
   }

   *(odata+(*olen)) = idata;
   (*olen)++;

   return(0);
}


/**********************************************************************/
/* Stores a vector of bytes of specified length to the output buffer. */
/**********************************************************************/
int putc_bytes(
   unsigned char *idata,  /* input buffer of bytes           */
   const int ilen,        /* bytes to be copied              */
   unsigned char *odata,  /* output buffer of bytes          */
   const int oalloc,      /* allocated size of output buffer */
   int *olen)             /* filled length of output buffer  */
{
   /* olen points to next position in odata */
   /* If output buffer is out of space ...  */
   if(((*olen)+ilen) > oalloc){
      fprintf(stderr,
      "ERROR : putc_bytes : buffer overlow : alloc = %d, request = %d\n",
      oalloc, (*olen)+ilen);
      return(-33);
   }

   memcpy(odata+(*olen), idata, ilen);
   (*olen) += ilen;

   return(0);
}


int putc_ushort(
   unsigned short ishort,     /* input unsigned short     */
   unsigned char *odata,      /* output byte buffer       */
   const int oalloc,          /* allocated size of buffer */
   int *olen)                 /* filled length of buffer  */
{
   int ret;
   unsigned char *cptr;

#ifdef __NBISLE__
   swap_short_bytes(ishort);
#endif

   cptr = (unsigned char *)(&ishort);

   if((ret = putc_bytes(cptr, sizeof(unsigned short), odata, oalloc, olen)))
      return(ret);

   return(0);
}


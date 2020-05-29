/********************************************************/
/* Writes huffman table to the compressed memory buffer */
/********************************************************/
int putc_huffman_table(
   const unsigned short marker,  /* Markers are different for JPEGL and WSQ */
   const unsigned char table_id,   /* huffman table indicator  */
   unsigned char *huffbits,      /* huffman table parameters */
   unsigned char *huffvalues,
   unsigned char *outbuf,        /* output byte buffer       */
   const int outalloc,   /* allocated size of buffer */
   int   *outlen)        /* filled length of buffer  */
{
   int i, ret;
   unsigned short table_len, values_offset;

   if(debug > 0)
      fprintf(stdout, "Start writing huffman table.\n");

   /* DHT */
   if((ret = putc_ushort(marker, outbuf, outalloc, outlen)))
      return(ret);

   /* "value(2) + table id(1) + bits(16)" */
   table_len = values_offset = 3 + MAX_HUFFBITS;
   for(i = 0; i < MAX_HUFFBITS; i++)
      table_len += huffbits[i];   /* values size */

   if(debug > 1){
      fprintf(stdout, "Table Len = %d\n", table_len);
      fprintf(stdout, "Table ID = %d\n", table_id);
      for(i = 0; i < MAX_HUFFBITS; i++)
         fprintf(stdout, "bits[%d] = %d\n", i, huffbits[i]);
      for(i = 0; i < table_len-values_offset; i++)
         fprintf(stdout, "values[%d] = %d\n", i, huffvalues[i]);
   }

   /* Table Len */
   if((ret = putc_ushort(table_len, outbuf, outalloc, outlen)))
      return(ret);

   /* Table ID */
   if((ret = putc_byte(table_id, outbuf, outalloc, outlen)))
      return(ret);

   /* Huffbits (MAX_HUFFBITS) */
   for(i = 0; i < MAX_HUFFBITS; i++){
      if((ret = putc_byte(huffbits[i], outbuf, outalloc, outlen)))
         return(ret);
   }

   /* Huffvalues (MAX_HUFFCOUNTS) */
   for(i = 0; i < table_len-values_offset; i++){
      if((ret = putc_byte(huffvalues[i], outbuf, outalloc, outlen)))
         return(ret);
   }

   if(debug > 0)
      fprintf(stdout, "Finished writing huffman table.\n\n");

   return(0);
}

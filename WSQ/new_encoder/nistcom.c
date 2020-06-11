/*combine_wsq_nistcom - takes an initialized FET NISTCOM structure
             or allocates one if necessary, and updates the
            structure with WSQ-specific image attributes.
    combine_nistcom
    updatefet_ret
    lookupfet


fet2string - takes an FET structure and concatenates (name,value)
            pairs into a single null-terminated string with each
                         (name,value) pair delimited by a new-line.

string2fet - parses a null-terminated string representing a
              list of (name,value) pairs into an FET structure.

putc_comment - Writes a text string out as a JPEGL comment block
                   to a memory buffer.*/

#include "huff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nistcom.h"
#include "dataio.h"

/*****************************************************************/
int combine_wsq_nistcom(NISTCOM **onistcom, const int w, const int h,
                  const int d, const int ppi, const int lossyflag,
                  const float r_bitrate)
{
   int ret, allocflag;
   NISTCOM *nistcom;
   char cbuff[MAXFETLENGTH];

   if(*onistcom == (NISTCOM *)NULL)
      allocflag = 1;
   else
      allocflag = 0;

   /* Combine image attributes to NISTCOM. */
   if((ret = combine_nistcom(onistcom, w, h, d, ppi, lossyflag)))
      return(ret);

   nistcom = *onistcom;

   /* COLORSPACE */
   if((ret = updatefet_ret(NCM_COLORSPACE, "GRAY", nistcom))){
      if(allocflag)
         freefet(nistcom);
      return(ret);
   }

   /* COMPRESSION */
   if((ret = updatefet_ret(NCM_COMPRESSION, "WSQ", nistcom))){
      if(allocflag)
         freefet(nistcom);
      return(ret);
   }

   /* BITRATE */
   sprintf(cbuff, "%f", r_bitrate);
   if((ret = updatefet_ret(NCM_WSQ_RATE, cbuff, nistcom))){
      if(allocflag)
         freefet(nistcom);
      return(ret);
   }

   /* UPDATE HEADER */
   sprintf(cbuff, "%d", nistcom->num);
   if((ret = updatefet_ret(NCM_HEADER, cbuff, nistcom))){
      if(allocflag)
         freefet(nistcom);
      return(ret);
   }

   return(0);
}





/*****************************************************************/
int combine_nistcom(NISTCOM **onistcom, const int w, const int h,
                     const int d, const int ppi, const int lossyflag)
{
   int ret, allocflag, n;
   char *lossyval;
   char cbuff[11];
   NISTCOM *nistcom;


   /* ALLOCATION ? */
   if((*onistcom) == (NISTCOM *)NULL){
      if((ret = allocfet_ret(&nistcom, 6)))
         return(ret);
      allocflag = 1;
      /* HEADER */
      if((ret = updatefet_ret(NCM_HEADER, "6", nistcom))){
         if(allocflag){
            freefet(nistcom);
            *onistcom = (NISTCOM *)NULL;
         }
         return(ret);
      }
   }
   else{
      nistcom = *onistcom;
      allocflag = 0;
      n = 6;
   }

   /* WIDTH */
   sprintf(cbuff, "%d", w);
   if((ret = updatefet_ret(NCM_PIX_WIDTH, cbuff, nistcom))){
      if(allocflag){
         freefet(nistcom);
         *onistcom = (NISTCOM *)NULL;
      }
      return(ret);
   }

   /* HEIGHT */
   sprintf(cbuff, "%d", h);
   if((ret = updatefet_ret(NCM_PIX_HEIGHT, cbuff, nistcom))){
      if(allocflag){
         freefet(nistcom);
         *onistcom = (NISTCOM *)NULL;
      }
      return(ret);
   }

   /* DEPTH */
   sprintf(cbuff, "%d", d);
   if((ret = updatefet_ret(NCM_PIX_DEPTH, cbuff, nistcom))){
      if(allocflag){
         freefet(nistcom);
         *onistcom = (NISTCOM *)NULL;
      }
      return(ret);
   }

   /* PPI */
   sprintf(cbuff, "%d", ppi);
   if((ret = updatefet_ret(NCM_PPI, cbuff, nistcom))){
      if(allocflag){
         freefet(nistcom);
         *onistcom = (NISTCOM *)NULL;
      }
      return(ret);
   }

   /* LOSSY */
   /* If exists, lookup current LOSSY value. */
   ret = lookupfet(&lossyval, NCM_LOSSY, nistcom);
   /* If error ... */
   if(ret < 0){
      if(allocflag){
         freefet(nistcom);
         *onistcom = (NISTCOM *)NULL;
      }
      return(ret);
   }
   /* If LOSSY value found AND is set AND requesting to unset ... */
   if(ret &&
     (strcmp(lossyval,"0") != 0) &&
     (lossyflag == 0)){
      fprintf(stderr, "WARNING : combine_nistcom : ");
      fprintf(stderr, "request to unset lossy flag ignored\n");
   }
   else{
      sprintf(cbuff, "%d", lossyflag);
      if((ret = updatefet_ret(NCM_LOSSY, cbuff, nistcom))){
         if(allocflag){
            freefet(nistcom);
            *onistcom = (NISTCOM *)NULL;
         }
         return(ret);
      }
   }

   /* UPDATE HEADER */
   sprintf(cbuff, "%d", nistcom->num);
   if((ret = updatefet_ret(NCM_HEADER, cbuff, nistcom))){
      if(allocflag){
         freefet(nistcom);
         *onistcom = (NISTCOM *)NULL;
      }
      return(ret);
   }

   *onistcom = nistcom;

   return(0);
}


/*from lkupfet.c file*/
int lookupfet(char **ovalue, char *feature, FET *fet)
{
  int item;
  char *value;
  size_t len = 0;

  for (item = 0;
       (item < fet->num) && (strcmp(fet->names[item],feature) != 0);
       item++);
  if (item>=fet->num){
     return(FALSE);
  }
  if(fet->values[item] != (char *)NULL){
      len = strlen(fet->values[item]) + 1;
      value = malloc(len);
      if (value == (char *)NULL){
         fprintf(stderr, "ERROR : lookupfet : malloc : value\n");
         return(-2);
     }
     strncpy(value, fet->values[item], len);
  }
  else
      value = (char *)NULL;

  *ovalue = value;

  return(TRUE);
}


/*from updatefet.c file*/

/***********************************************************************/
int updatefet_ret(char *feature, char *value, FET *fet)
{
  int ret, item;
  int increased, incr;
  size_t len = 0;

  for (item = 0;
       (item < fet->num) && (strcmp(fet->names[item],feature) != 0);
       item++);
  if (item < fet->num){
     if(fet->values[item] != (char *)NULL){
        free(fet->values[item]);
        fet->values[item] = (char *)NULL;
     }
     if(value != (char *)NULL){
        len = strlen(value) + 1;
        fet->values[item] = malloc(len);
        if(fet->values[item] == (char *)NULL){
           fprintf(stderr, "ERROR : updatefet_ret : malloc : fet->values[]\n");
           return(-2);
        }
	strncpy(fet->values[item], value, len);
     }
  }
  else{
     if(fet->num >= fet->alloc){
        incr      = fet->alloc / 10;		/* add 10% or 10 which-	*/
        increased = fet->alloc + max(10, incr);	/* ever is larger	*/
        if((ret = reallocfet_ret(&fet, increased)))
           return(ret);
     }
     len = strlen(feature) + 1;
     fet->names[fet->num] = malloc(len);
     if(fet->names[fet->num] == (char *)NULL){
        fprintf(stderr, "ERROR : updatefet_ret : malloc : fet->names[]\n");
        return(-3);
     }
     strncpy(fet->names[fet->num], feature, len);
     if(value != (char *)NULL){
     	len = strlen(value) + 1;
        fet->values[fet->num] = malloc(len);
        if(fet->values[fet->num] == (char *)NULL){
           fprintf(stderr, "ERROR : updatefet_ret : malloc : fet->values[]\n");
           return(-4);
        }
	strncpy(fet->values[fet->num], value, len);
     }
     (fet->num)++;
  }

  return(0);
}





int fet2string(char **ostr, FET *fet)
{
   int i, size;
   char *str;

   /* Calculate size of string. */
   size = 0;
   for(i = 0; i < fet->num; i++){
      size += strlen(fet->names[i]);
      size += strlen(fet->values[i]);
      size += 2;
   }
   /* Make room for NULL for final strlen() below. */
   size++;

   if((str = (char *)calloc(size, sizeof(char))) == (char *)NULL){
      fprintf(stderr, "ERROR : fet2string : malloc : str\n");
      return(-2);
   }

   for(i = 0; i < fet->num; i++){
      strcat(str, fet->names[i]);
      strcat(str, " ");
      strcat(str, fet->values[i]);
      strcat(str, "\n");
   }

   str[strlen(str)-1] = '\0';

   *ostr = str;
   return(0);
}


/*****************************************************************/
int string2fet(FET **ofet, char *istr)
{
   int ret;
   char *iptr, *optr;
   char name[MAXFETLENGTH], value[MAXFETLENGTH], *vptr;
   FET *fet;

   if((ret = allocfet_ret(&fet, MAXFETS)))
      return(ret);

   iptr = istr;
   while(*iptr != '\0'){
      /* Get next name */
      optr = name;
      while((*iptr != '\0')&&(*iptr != ' ')&&(*iptr != '\t'))
         *optr++ = *iptr++;
      *optr = '\0';

      /* Skip white space */
      while((*iptr != '\0')&&
            ((*iptr == ' ')||(*iptr == '\t')))
         iptr++;

      /* Get next value */
      optr = value;
      while((*iptr != '\0')&&(*iptr != '\n'))
         *optr++ = *iptr++;
      *optr = '\0';

      /* Skip white space */
      while((*iptr != '\0')&&
            ((*iptr == ' ')||(*iptr == '\t')||(*iptr == '\n')))
         iptr++;

      /* Test (name,value) pair */
      if(strlen(name) == 0){
         fprintf(stderr, "ERROR : string2fet : empty name string found\n");
         return(-2);
      }
      if(strlen(value) == 0)
         vptr = (char *)NULL;
      else
         vptr = value;

      /* Store name and value pair into FET. */
      if((ret = updatefet_ret(name, vptr, fet))){
         freefet(fet);
         return(ret);
      }
   }

   *ofet = fet;
   return(0);
}

/****************************************/
/* Puts comment field in output buffer. */
/****************************************/
int putc_comment(
   const unsigned short marker,
   unsigned char *comment,        /* comment */
   const int cs,      /* comment size */
   unsigned char *odata,      /* output byte buffer       */
   const int oalloc,  /* allocated size of buffer */
   int   *olen)       /* filled length of buffer  */
{
   int ret, i;
   unsigned short hdr_size;              /* header size */

   if(debug > 0)
      fprintf(stderr, "Writing Comment Field to Buffer.\n");

   if((ret = putc_ushort(marker, odata, oalloc, olen)))
      return(ret);
   /* comment size */
   hdr_size = 2 + cs;
   if((ret = putc_ushort(hdr_size, odata, oalloc, olen)))
      return(ret);
   for(i = 0; i < cs; i++)
      if((ret = putc_byte(comment[i], odata, oalloc, olen)))
         return(ret);

   if(debug > 0)
      fprintf(stderr, "Finished Writing Comment Field to Buffer.\n");

   return(0);
}


void freefet(FET *fet)
{
  int item;
  for (item=0;item<fet->num;item++){
      free (fet->names[item]);
      free (fet->values[item]);
  }
  free((char *)fet->names);
  free((char *)fet->values);
  free(fet);
}



/********************************************************************/
int allocfet_ret(FET **ofet, int numfeatures)
{
   FET *fet;

   fet = (FET *)malloc(sizeof(FET));
   if (fet == (FET *)NULL){
      fprintf(stderr, "ERROR : allocfet_ret : malloc : fet\n");
      return(-2);
   }
   /* calloc here is required */
   fet->names = (char **)calloc(numfeatures, sizeof(char *));
   if (fet->names == (char **)NULL){
      fprintf(stderr, "ERROR : allocfet_ret : calloc : fet->names\n");
      free(fet);
      return(-3);
   }
   fet->values = (char **)calloc(numfeatures, sizeof(char *));
   if (fet->values == (char **)NULL){
      fprintf(stderr, "ERROR : allocfet_ret : calloc : fet->values\n");
      free(fet->names);
      free(fet);
      return(-4);
   }
   fet->alloc = numfeatures;
   fet->num = 0;

   *ofet = fet;

   return(0);
}


/********************************************************************/
int reallocfet_ret(FET **ofet, int newlen)
{
   int ret;
   FET *fet;

   fet = *ofet;

   /* If fet not allocated ... */
   if ((fet == (FET *)NULL || fet->alloc == 0)){
      /* Allocate the fet. */
      if((ret = allocfet_ret(ofet, newlen)))
         /* Return error code. */
         return(ret);
      /* Otherwise allocation was successful. */
      return(0);
   }

   /* Oherwise, reallocate fet. */
   fet->names = (char **)realloc(fet->names, newlen * sizeof(char *));
   if (fet->names == (char **)NULL){
      fprintf(stderr, "ERROR : reallocfet_ret : realloc : fet->names\n");
      return(-2);
   }
   fet->values = (char **)realloc(fet->values, newlen * sizeof(char *));
   if (fet->values == (char **)NULL){
      fprintf(stderr, "ERROR : reallocfet_ret : realloc : fet->values");
      return(-3);
   }
   fet->alloc = newlen;

   return(0);
}


/*******************************************************************/
int extractfet_ret(char **ovalue, char *feature, FET *fet)
{
  int item;
  char *value;
  size_t len = 0;

  for (item = 0;
       (item < fet->num) && (strcmp(fet->names[item],feature) != 0);
       item++);
  if (item>=fet->num){
     fprintf(stderr, "ERROR : extractfet_ret : feature %s not found\n",
             feature);
     return(-2);
  }
  if(fet->values[item] != (char *)NULL){
      len = strlen(fet->values[item]) + 1;
      value = malloc(len);
      if (value == (char *)NULL){
         fprintf(stderr, "ERROR : extractfet_ret : malloc : value\n");
         return(-3);
     }
     strncpy(value, fet->values[item], len);
  }
  else
      value = (char *)NULL;

  *ovalue = value;

  return(0);
}



/*void syserr(char *funcname, char *syscall, char *msg)
{

   (void) fflush(stdout);
   if(msg == NULL)
      (void) fprintf(stderr,"ERROR: %s: %s\n",funcname,syscall);
   else
      (void) fprintf(stderr,"ERROR: %s: %s: %s\n",funcname,syscall,msg);
   (void) fflush(stderr);

   exit(-1);
}*/

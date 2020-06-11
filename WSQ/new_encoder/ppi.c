#include <stdio.h>
#include <stdlib.h>
#include "wsq.h"


/************************************************************************/
int getc_ppi_wsq(int *oppi, unsigned char *idata, const int ilen)
{
   int ret;
   int ppi;
   char *value;
   NISTCOM *nistcom;

   /* Get ppi from NISTCOM, if one exists ... */
   if((ret = getc_nistcom_wsq(&nistcom, idata, ilen)))
      return(ret);
   if(nistcom != (NISTCOM *)NULL){
      if((ret = extractfet_ret(&value, NCM_PPI, nistcom))){
         freefet(nistcom);
         return(ret);
      }
      if(value != (char *)NULL){
         ppi = atoi(value);
         free(value);
      }
      /* Otherwise, PPI not in NISTCOM, so ppi = -1. */
      else
         ppi = -1;
      freefet(nistcom);
   }
   /* Otherwise, NISTCOM does NOT exist, so ppi = -1. */
   else
      ppi = -1;

   *oppi = ppi;

   return(0);
}


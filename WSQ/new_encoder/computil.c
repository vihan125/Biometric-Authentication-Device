#include <stdio.h>
#include "dataio.h"
#include "computil.h"

/*****************************************************************/
/* Skips the segment data following a JPEGB, JPEGL, or WSQ       */
/* marker in the given memory buffer.                            */
/*****************************************************************/
int getc_skip_marker_segment(const unsigned short marker,
                     unsigned char **cbufptr, unsigned char *ebufptr)
{
   int ret;
   unsigned short length;

   /* Get ushort Length. */
   if((ret = getc_ushort(&length, cbufptr, ebufptr)))
      return(ret);

   length -= 2;

   /* Check for EOB ... */
   if(((*cbufptr)+length) >= ebufptr){
      fprintf(stderr, "ERROR : getc_skip_marker_segment : ");
      fprintf(stderr, "unexpected end of buffer when parsing ");
      fprintf(stderr, "marker %d segment of length %d\n", marker, length);
      return(-2);
   }

   /* Bump buffer pointer. */
   (*cbufptr) += length;

   return(0);
}


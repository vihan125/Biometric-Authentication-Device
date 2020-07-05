#include <stdio.h>
#include <stdlib.h>

#ifndef True
#define True		1
#define False		0
#endif
#define FET_EXT		"fet"
#define MAXFETS		100
#define MAXFETLENGTH	512

typedef struct fetstruct{
   int alloc;
   int num;
   char **names;
   char **values;
} FET;

typedef FET NISTCOM;


#define NCM_EXT         "ncm"
#define NCM_HEADER      "NIST_COM"        /* manditory */
#define NCM_PIX_WIDTH   "PIX_WIDTH"       /* manditory */
#define NCM_PIX_HEIGHT  "PIX_HEIGHT"      /* manditory */
#define NCM_PIX_DEPTH   "PIX_DEPTH"       /* 1,8,24 (manditory)*/
#define NCM_PPI         "PPI"             /* -1 if unknown (manditory)*/
#define NCM_COLORSPACE  "COLORSPACE"      /* RGB,YCbCr,GRAY */
#define NCM_N_CMPNTS    "NUM_COMPONENTS"  /* [1..4] (manditory w/hv_factors)*/
#define NCM_HV_FCTRS    "HV_FACTORS"      /* H0,V0:H1,V1:...*/
#define NCM_INTRLV      "INTERLEAVE"      /* 0,1 (manditory w/depth=24) */
#define NCM_COMPRESSION "COMPRESSION"     /* NONE,JPEGB,JPEGL,WSQ */
#define NCM_JPEGB_QUAL  "JPEGB_QUALITY"   /* [20..95] */
#define NCM_JPEGL_PREDICT "JPEGL_PREDICT" /* [1..7] */
#define NCM_WSQ_RATE    "WSQ_BITRATE"     /* ex. .75,2.25 (-1.0 if unknown)*/
#define NCM_LOSSY       "LOSSY"           /* 0,1 */

#define NCM_HISTORY     "HISTORY"         /* ex. SD historical data */
#define NCM_FING_CLASS  "FING_CLASS"      /* ex. A,L,R,S,T,W */
#define NCM_SEX         "SEX"             /* m,f */
#define NCM_SCAN_TYPE   "SCAN_TYPE"       /* l,i */
#define NCM_FACE_POS    "FACE_POS"        /* f,p */
#define NCM_AGE         "AGE"
#define NCM_SD_ID       "SD_ID"





/*********************************************************************/
/*          General Purpose Defines                                  */
/*********************************************************************/
#ifndef True
#define True		1
#define False		0
#endif
#ifndef TRUE
#define TRUE		True
#define FALSE		False
#endif
#define Yes		True
#define No		False
#define Empty		NULL
#ifndef None
#define None		-1
#endif
#ifndef FOUND
#define FOUND            1
#endif
#define NOT_FOUND_NEG   -1
#define EOL		EOF
#ifndef DEG2RAD
#define DEG2RAD	(double)(57.29578)
#endif
#define max(a, b)   ((a) > (b) ? (a) : (b))
#define min(a, b)   ((a) < (b) ? (a) : (b))
#define sround(x) ((int) (((x)<0) ? (x)-0.5 : (x)+0.5))
#define sround_uint(x) ((unsigned int) (((x)<0) ? (x)-0.5 : (x)+0.5))
#define align_to_16(_v_)   ((((_v_)+15)>>4)<<4)
#define align_to_32(_v_) ((((_v_)+31)>>5)<<5)
#ifndef CHUNKS
#define CHUNKS          100
#endif


extern int combine_nistcom(NISTCOM **, const int, const int,const int, const int, const int);
extern int combine_wsq_nistcom(NISTCOM **, const int, const int,const int, const int, const int, const float);
extern int  lookupfet(char **, char *, FET *);
extern int fet2string(char **, FET *);
extern int string2fet(FET **, char *);
extern void updatefet(char *, char *, FET *);
extern int  updatefet_ret(char *, char *, FET *);
extern int putc_comment(const unsigned short, unsigned char *, const int,unsigned char *, const int, int *);
extern void freefet(FET *fet);
extern int allocfet_ret(FET **ofet, int numfeatures);
extern int reallocfet_ret(FET **ofet, int newlen);
extern int  extractfet_ret(char **, char *, FET *);

#define SOF3 0xffc3
#define DHT  0xffc4
#define RST0 0xffd0
#define RST1 0xffd1
#define RST2 0xffd2
#define RST3 0xffd3
#define RST4 0xffd4
#define RST5 0xffd5
#define RST6 0xffd6
#define RST7 0xffd7
#define SOI  0xffd8
#define EOI  0xffd9
#define SOS  0xffda
#define DNL  0xffdc
#define DRI  0xffdd
#define COM  0xfffe
#define APP0 0xffe0
/* Case for getting ANY marker. */
#define ANY  0xffff
/* Cases for getting a table from a set of possible ones. */
#define TBLS_N_SOF     2
#define TBLS_N_SOS     (TBLS_N_SOF + 1)

/* Predictor Definitions */
/*    c b    */
/*    a x    */
#define   PRED1       1 /* Px = Ra */
#define   PRED2       2 /* Px = Rb */
#define   PRED3       3 /* Px = Rc */
#define   PRED4       4 /* Px = Ra+Rb-Rc */
#define   PRED5       5 /* Px = Ra+((Rb-Rc)/2) */
#define   PRED6       6 /* Px = Rb+((Ra-Rc)/2) */
#define   PRED7       7 /* Px = (Ra+Rb)/2 */
#define   BITSET      0x01
#define   LSBITMASK   0x0001
#define   CATMASK     0x8000

#define   NO_INTRLV      0
#define   MAX_CMPNTS 4
#define   FREE_IMAGE     1
#define   NO_FREE_IMAGE  0
#define   BUF_SIZE       50000   /* Compressed image buffer size */

#define   MIN_HUFFTABLE_ID 16  /* Set according to JPEGL spec */
#define   MAX_HUFFBITS     16  /* DO NOT CHANGE THIS CONSTANT!! */
#define   MAX_HUFFCOUNTS_JPEGL 16  /* Length of code table: change as */
                                   /* needed but DO NOT EXCEED 256    */
#define   MAX_CATEGORY  10    /* Largest difference category for uchar data */
#define   LARGESTDIFF   511   /* Largest difference value */

#define   READ_TABLE_LEN    1
#define   NO_READ_TABLE_LEN 0

#define   FIRSTBIT     7
#ifndef   BITSPERBYTE
#define   BITSPERBYTE  8
#endif

/* JFIF SCAN UNIT DESIGNATORS */
#define UNKNOWN_UNITS    0
#define PPI_UNITS       1  /* pixels per inch */
#define PPCM_UNITS      2  /* pixels per centimeter */




typedef struct hcode {
   short size;
   unsigned int code;
} HUFFCODE;


typedef struct htable {
   unsigned char def;
   unsigned char table_id;
   unsigned char *bits;
   unsigned char *values;
   int last_size;
   int *codesize;
   int *freq;
   int *maxcode;
   int *mincode;
   int *valptr;
   HUFFCODE *huffcode_table;
} HUF_TABLE;





int debug;



#include "huff.h"
#include "nistcom.h"

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

/* WSQ Marker Definitions */
#define SOI_WSQ 0xffa0
#define EOI_WSQ 0xffa1
#define SOF_WSQ 0xffa2
#define SOB_WSQ 0xffa3
#define DTT_WSQ 0xffa4
#define DQT_WSQ 0xffa5
#define DHT_WSQ 0xffa6
#define DRT_WSQ 0xffa7
#define COM_WSQ 0xffa8
/* Case for getting ANY marker. */
#define ANY_WSQ 0xffff
#define TBLS_N_SOB   (TBLS_N_SOF + 2)

/* Filter Bank Definitions */
#ifdef FILTBANK_EVEN_8X8_1
#define MAX_HIFILT   8
#define MAX_LOFILT   8
#else
#define MAX_HIFILT   7
#define MAX_LOFILT   9
#endif

/* Subband Definitions */
#define STRT_SUBBAND_2      19
#define STRT_SUBBAND_3      52
#define MAX_SUBBANDS        64
#define NUM_SUBBANDS        60
#define STRT_SUBBAND_DEL    (NUM_SUBBANDS)
#define STRT_SIZE_REGION_2  4
#define STRT_SIZE_REGION_3  51

#define MIN_IMG_DIM         256

#define WHITE               255
#define BLACK               0

#define COEFF_CODE          0
#define RUN_CODE            1

#define RAW_IMAGE           1
#define IHEAD_IMAGE         0

#define VARIANCE_THRESH     1.01

typedef struct quantization {
   float q;  /* quantization level */
   float cr; /* compression ratio */
   float r;  /* compression bitrate */
   float qbss_t[MAX_SUBBANDS];
   float qbss[MAX_SUBBANDS];
   float qzbs[MAX_SUBBANDS];
   float var[MAX_SUBBANDS];
} QUANT_VALS;

typedef struct wavlet_tree {
   int x;
   int y;
   int lenx;
   int leny;
   int inv_rw;
   int inv_cl;
} W_TREE;
#define W_TREELEN 20

typedef struct quant_tree {
   short x;     /* UL corner of block */
   short y;
   short lenx;  /* block size */
   short leny;  /* block size */
} Q_TREE;
#define Q_TREELEN 64

/* Defined in jpegl.h
typedef struct hcode {
   short  size;
   unsigned short code;*/

typedef struct table_dtt {
   float *lofilt;
   float *hifilt;
   unsigned char losz;
   unsigned char hisz;
   char lodef;
   char hidef;
} DTT_TABLE;

typedef struct table_dqt {
   float bin_center;
   float q_bin[MAX_SUBBANDS];
   float z_bin[MAX_SUBBANDS];
   char dqt_def;
} DQT_TABLE;

#define MAX_DHT_TABLES  8

/* Defined in jpegl.h */
/* #define MAX_HUFFBITS      16  DO NOT CHANGE THIS CONSTANT!! */
#define MAX_HUFFCOUNTS_WSQ  256  /* Length of code table: change as needed */
                                 /* but DO NOT EXCEED 256 */
#define MAX_HUFFCOEFF        74  /* -73 .. +74 */
#define MAX_HUFFZRUN        100

typedef struct table_dht {
   unsigned char tabdef;
   unsigned char huffbits[MAX_HUFFBITS];
   unsigned char huffvalues[MAX_HUFFCOUNTS_WSQ+1];
} DHT_TABLE;

typedef struct header_frm {
   unsigned char black;
   unsigned char white;
   unsigned short width;
   unsigned short height;
   float m_shift;
   float r_scale;
   unsigned char wsq_encoder;
   unsigned short software;
} FRM_HEADER_WSQ;

/* External global variables. */
extern int debug;
extern QUANT_VALS quant_vals;
extern W_TREE w_tree[];
extern Q_TREE q_tree[];
extern DTT_TABLE dtt_table;
extern DQT_TABLE dqt_table;
extern DHT_TABLE dht_table[];
extern FRM_HEADER_WSQ frm_header_wsq;
extern float hifilt[];
extern float lofilt[];

/* decoder.c */
extern int wsq_decode_mem(unsigned char **, int *, int *, int *, int *, int *,unsigned char *, const int);
extern int huffman_decode_data_mem(short *, DTT_TABLE *, DQT_TABLE *,DHT_TABLE *, unsigned char **, unsigned char *);
extern int decode_data_mem(int *, int *, int *, int *, unsigned char *,unsigned char **, unsigned char *, int *, unsigned short *);
extern int getc_nextbits_wsq(unsigned short *, unsigned short *,unsigned char **, unsigned char *, int *, const int);


/*encoder.c*/
extern int wsq_encode_mem(unsigned char **, int *, const float, unsigned char *,const int, const int, const int, const int, char *);
extern int gen_hufftable_wsq(HUFFCODE **, unsigned char **, unsigned char **,short *, const int *, const int);
extern int compress_block(unsigned char *, int *, short *,const int, const int, const int, HUFFCODE *);
extern int count_block(int **, const int, short *,const int, const int, const int);

/*tableio.c*/
extern int putc_transform_table(float *, const int, float *, const int,unsigned char *, const int, int *);
extern int putc_quantization_table(QUANT_VALS *, unsigned char *, const int,int *);
extern int putc_frame_header_wsq(const int, const int, const float,const float, unsigned char *, const int, int *);
extern int putc_nistcom_wsq(char *, const int, const int, const int,const int, const int, const float, unsigned char *,const int, int *);
extern int putc_block_header(const int, unsigned char *, const int, int *);
extern int getc_marker_wsq(unsigned short *, const int, unsigned char **,unsigned char *);
extern int getc_table_wsq(unsigned short, DTT_TABLE *, DQT_TABLE *, DHT_TABLE *,unsigned char **, unsigned char *);
extern int getc_frame_header_wsq(FRM_HEADER_WSQ *, unsigned char **,unsigned char *);
extern int getc_block_header(unsigned char *, unsigned char **,unsigned char *);
extern int getc_transform_table(DTT_TABLE *, unsigned char **, unsigned char *);
extern int getc_quantization_table(DQT_TABLE *, unsigned char **,unsigned char *);
extern int getc_huffman_table_wsq(DHT_TABLE *, unsigned char **,unsigned char *);
extern int getc_comment(unsigned char **, unsigned char **, unsigned char *);
extern int getc_nistcom_wsq(NISTCOM **, unsigned char *, const int);

/*tree.c*/
extern void build_wsq_trees(W_TREE w_tree[], const int,Q_TREE q_tree[], const int, const int, const int);
extern void build_q_tree(W_TREE w_tree[], Q_TREE q_tree[]);
extern void build_w_tree(W_TREE w_tree[], const int, const int);
extern void q_tree4(Q_TREE *q_tree,const int start,const int lenx,const int leny,const int x,const int  y);
extern void q_tree16(Q_TREE *q_tree,const int start,const int lenx,const int leny,const int x,const int y,const int rw,const int cl);
extern void w_tree4(W_TREE w_tree[],const int start1,const int start2,const int lenx,const int leny,const int x,const int y,const int stop1);

/*util.c*/
extern int conv_img_2_flt_ret(float *, float *, float *, unsigned char *,const int);
extern void variance( QUANT_VALS *quant_vals, Q_TREE q_tree[], const int,float *, const int, const int);
extern int quantize(short **, int *, QUANT_VALS *, Q_TREE qtree[], const int,float *, const int, const int);
extern void quant_block_sizes(int *, int *, int *,QUANT_VALS *, W_TREE w_tree[], const int,Q_TREE q_tree[], const int);
extern int wsq_decompose(float *, const int, const int,W_TREE w_tree[], const int, float *, const int,float *, const int);
extern void get_lets(float *new,float *old,const int len1,const int len2,const int pitch,const int  stride,float *hi,const int hsz,float *lo,const int lsz,const int inv);
extern void conv_img_2_uchar(unsigned char *, float *, const int, const int,const float, const float);
extern int unquantize(float **, const DQT_TABLE *,Q_TREE q_tree[], const int, short *, const int, const int);
extern void  join_lets(float *, float *, const int, const int,const int, const int, float *, const int,float *, const int, const int);
extern int wsq_reconstruct(float *, const int, const int,W_TREE w_tree[], const int, const DTT_TABLE *);
extern void init_wsq_decoder_resources(void);
extern void free_wsq_decoder_resources(void);
extern int int_sign(const int);

/*huff.c*/
extern int putc_huffman_table(const unsigned short, const unsigned char,unsigned char *, unsigned char *, unsigned char *,const int, int *);
extern int find_huff_sizes(int **ocodesize, int *freq, const int max_huffcounts);
extern int find_num_huff_sizes(unsigned char **obits, int *adjust, int *codesize,const int max_huffcounts);
extern int sort_huffbits(unsigned char *bits);
extern int sort_code_sizes(unsigned char **ovalues, int *codesize,const int max_huffcounts);
extern int build_huffcode_table(HUFFCODE **ohuffcode_table,HUFFCODE *in_huffcode_table, const int last_size,unsigned char *values, const int max_huffcounts);
extern int build_huffsizes(HUFFCODE **ohuffcode_table, int *temp_size,unsigned char *huffbits, const int max_huffcounts);
extern void build_huffcodes(HUFFCODE *huffcode_table);
extern int check_huffcodes_wsq(HUFFCODE *hufftable, int last_size);
extern void find_least_freq(int *value1, int *value2, int *freq,const int max_huffcounts);
extern void gen_decode_table(HUFFCODE *huffcode_table,int *maxcode, int *mincode, int *valptr, unsigned char *huffbits);
extern int getc_huffman_table(unsigned char *, unsigned char **,unsigned char **, const int, unsigned char **,unsigned char *, const int, int *);


/* ppi.c */
extern int getc_ppi_wsq(int *, unsigned char *, const int);

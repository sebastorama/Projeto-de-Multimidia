/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

typedef int quantizationMatrix[8][8];

/* Struct that store the values ​​of the quantization
   for each pixel. The byte order is the same that you'll
   find in the bitmap file */
typedef struct pixel_intValue {
	short int blue;
	short int green;
	short int red;
} pixel_intValue;

/* The 8x8 pixels block that will be used for further processing */
struct {
	pixel_intValue data[8][8];
} typedef pixel_block;

/* The struct that will hold a matrix of the 8x8 blocks. This is intended to
 have the exact same structure of the original matrix if you place the blocks
 in the correct order */
struct {
	int w_block;               /* Width size of the matrix of blocks */
	int h_block;               /* Height size of the matrix of blocks */
	pixel_block ** blocks;     /* The matrix of blocks :D */
} typedef intValue_blocks;
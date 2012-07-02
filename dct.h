/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

/* Struct that store the values of the discrete cosine 
   transform for each pixel. The byte order is the same that you'll
   find in the bitmap file */
typedef struct pixel_value {
	float blue;
	float green;
	float red;
} pixel_value;

/* The 8x8 pixels block that will be used for further processing */
struct {
	pixel_value data[8][8];
} typedef block;

/* The struct that will hold a matrix of the 8x8 blocks. This is intended to
 have the exact same structure of the original matrix if you place the blocks
 in the correct order */
struct {
	int w_block;       /* Width size of the matrix of blocks */
	int h_block;       /* Height size of the matrix of blocks */
	block ** blocks;   /* The matrix of blocks :D */
} typedef value_blocks;

value_blocks dct(pixel_blocks pb);
pixel_blocks idct(value_blocks vb);
value_blocks create_value_blocks(int w_block_count, int h_block_count);
pixel_blocks value_blocks_to_pixel_blocks(value_blocks vb);

/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

/* The 8x8 pixels block that will be used for further processing */
struct {
	BMP_pixel data[8][8];
} typedef block_8x8;

/* The struct that will hold a matrix of the 8x8 blocks. This is intended to
   have the exact same structure of the original matrix if you place the blocks
   in the correct order */
struct {
	int w_block_count;       /* Width size of the matrix of blocks */
	int h_block_count;       /* Height size of the matrix of blocks */
	block_8x8 ** blocks;     /* The matrix of blocks :D */
} typedef pixel_blocks;

/* Get the BMP_pixel_matrix (see bmp.h documentation) and rearrange it into 
   the 8x8_blocks (the pixel_blocks type) */
pixel_blocks create_pixel_blocks(int w_block_count, int h_block_count);
pixel_blocks BMP_pixel_matrix_2_pixel_blocks(BMP_pixel_matrix m);

BMP_pixel_matrix pixel_blocks_2_BMP_pixel_matrix(pixel_blocks pb);

/* You give a block, this print it's pixel data. Usefull just for debugging 
   purposes only */
void print_block(block_8x8 b);


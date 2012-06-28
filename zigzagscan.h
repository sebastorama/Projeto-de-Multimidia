/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

/* Vector of pixels used for vectorization of each 8x8 block. */
typedef struct {
	BMP_pixel data[64];
} vector;

/* Do zigzagscan on all the blocks in pb returning a 
 * bunch of serialized vectors
 */
vector * do_zigzagscan(pixel_blocks pb);

/* Get serialized vectors, the original blocks
 * matrix width and height information,
 * and reassemble the pixel_blocks structure
 */
pixel_blocks undo_zigzagscan(vector * v, int w_block_count, int h_block_count);


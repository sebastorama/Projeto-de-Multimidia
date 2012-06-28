/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */
#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"
#include "block_division.h"
#include "zigzagscan.h"

/* Create a pixel_block allocating the memory to hold the pixel data, given
   the size of the block matrix on 'w_block_count' and 'h_block_count' parameters
   
   The size of the resulting blocks is fixed, 8x8 pixels
   */
pixel_blocks create_pixel_blocks(int w_block_count, int h_block_count) {
	pixel_blocks result;
	
	/* Setup the struct */
	result.w_block_count = w_block_count;
	result.h_block_count = h_block_count;
	
	/* Let's allocate the matrix and fill with zeros */
	result.blocks = calloc(h_block_count, sizeof(block_8x8*));
	int i;
	for(i=0; i<h_block_count; i++) 
		result.blocks[i] = calloc(w_block_count, sizeof(block_8x8));

	return result;
}

/* Given a pixel, the x/y position on the original Bitmap matrix, and the
   pixel_blocks 'b', figure out which block and position inside this block
   contained on the matrix of blocks inside 'b' to insert this pixel, than
   insert it */
void set_pixel_on_blocks(BMP_pixel pixel, int x, int y, pixel_blocks * b) {
     /* Let's figure out which block */
	int block_row = y/8;
	int block_column = x/8;

    /* Now just insert on the correct position inside de block */
	b->blocks[block_row][block_column].data[y%8][x%8] = pixel;
}

/* Given a (x,y) coordinate, and a pixel_block(see block_division.h) 'b', extract
   the pixel from 'b' */
BMP_pixel get_pixel_from_blocks(int x, int y, pixel_blocks * b) {
	int block_row = y/8;
	int block_column = x/8;

	return b->blocks[block_row][block_column].data[y%8][x%8];
}


pixel_blocks BMP_pixel_matrix_2_pixel_blocks(BMP_pixel_matrix m) {
  /* Count the size of the matrix of blocks */
	int w_block_count = m.width/8;
	int h_block_count = m.height/8;
	
	/* Allocate my structure */
	pixel_blocks result = create_pixel_blocks(w_block_count, h_block_count);
	
	/* Run through the original pixel matrix copying to the blocks structure */
	int i,j;
	for(i=0; i<m.height; i++)
		for(j=0; j<m.width; j++)
			set_pixel_on_blocks(m.pixels[i][j], j, i, &result);

	return result;
}

BMP_pixel_matrix pixel_blocks_2_BMP_pixel_matrix(pixel_blocks pb) {
	/* Create the pixel matrix and set its dimensions */
	BMP_pixel_matrix result;
	result.width = pb.w_block_count*8;
	result.height = pb.h_block_count*8;

	/* Allocate the pixel matrix */
	result.pixels = calloc(result.height, sizeof(BMP_pixel*));

	int i, j;
	/* Run through the pixel blocks retrieving the pixel values */
	for(i=0; i<result.height; i++) {
		result.pixels[i] = calloc(result.width, sizeof(BMP_pixel));
		for(j=0; j<result.width; j++) {
			result.pixels[i][j] = get_pixel_from_blocks(j, i, &pb);
		}
	}

	return result;
}


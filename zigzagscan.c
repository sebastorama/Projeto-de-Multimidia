/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "block_division.h"
#include "zigzagscan.h"

#define SQUARE_SIZE 8

/* Go right if you can */
void go_right(int* i, int* j) {
	if(*j < SQUARE_SIZE - 1) *j += 1;
}

/* Go left if you can */
void go_left(int* i, int* j) {
	if(*j > 0) *j -= 1;
}

/* Go down if you can */
void go_down(int* i, int *j) {
	if(*i < SQUARE_SIZE - 1) *i += 1;
}

/* Go up if you can */
void go_up(int* i, int *j) {
	if(*i>0) *i -= 1;
}

/* Try the upright movement of the zigzagscan */
void try_upright(int* i, int *j) {
	if(*j < SQUARE_SIZE - 1) {
		go_up(i, j);
		go_right(i, j);
	} else { /* go down if we hit the right border */
		go_down(i, j);
	}
}

/* Try the downleft movement of the zigzagscan */
void try_downleft(int* i, int *j) {
	if(*i < SQUARE_SIZE - 1) {
		go_down(i, j);
		go_left(i, j);
	} else { /* go right if we hit the bottom border */
		go_right(i,j);
	}
}

void get_next_zig_zag_index(int *i, int *j) {
	if( (*i+*j)%2 == 0 ) {
		/* even diagonals should be upright */
		try_upright(i,j);
	} else {
		/* odd diagonals should be downleft */
		try_downleft(i,j);
	}
}

/* Convert a block_8x8 to a vector */
vector block_8x8_to_vector(block_8x8 b) {
	vector result;
	int i = 0;
	int j = 0;
	int k = 0;

	/* Just find whats the next zigzag index
	 * on the matrix, retrieve its value on
	 * a serial fashion :D
	 */
	for(k=0; k<64; k++) {
		result.data[k] = b.data[i][j];
		get_next_zig_zag_index(&i, &j);
	}
	return result;
}

/* Convert a vector to a block_8x8 */
block_8x8 vector_to_block_8x8(vector v) {
	block_8x8 result;
	int i = 0;
	int j = 0;
	int k = 0;

	/* Just find where to put the serialized
	 * calculating the next zigzag index
	 * to build the block
	 */
	for(k=0; k<64; k++) {
		result.data[i][j] = v.data[k];
		get_next_zig_zag_index(&i, &j);
	}
	return result;
}

vector * do_zigzagscan(pixel_blocks pb) {
	/* Allocate the vectors */
	vector* result = calloc(
			pb.w_block_count*pb.h_block_count,
			sizeof(vector)
		);

	int i, j, k;
	k = 0;
	for (i = 0; i < pb.h_block_count; i++)
		for (j = 0; j < pb.w_block_count; j++) {
			/* Apply the zigzag on each 8x8 pixels block */
			result[k] = block_8x8_to_vector(pb.blocks[i][j]);
			k++;
		}
	return result;
}

pixel_blocks undo_zigzagscan(vector * vectors, int w_block_count, int h_block_count) {
	/* Create our pixel blocks structure */
	pixel_blocks result = create_pixel_blocks(w_block_count, h_block_count);
	int i, j, k;
	k = 0;
	for (i = 0; i < result.h_block_count; i++)
		for (j = 0; j < result.w_block_count; j++) {
			/* For each block, reverse the zigzag scan */
			result.blocks[i][j] = vector_to_block_8x8(vectors[k]);
			k++;
		}

	return result;
}

/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "block_division.h"
#include "zigzagscan.h"
#include "dct.h"
#include "quantization.h"

/* Create a pixel_block allocating the memory to hold the pixel data, given
 the size of the block matrix on 'w_block_count' and 'h_block_count' parameters
 
 The size of the resulting blocks is fixed, 8x8 pixels
 */
intValue_blocks create_intValue_blocks(int w_block_count, int h_block_count) {
	intValue_blocks result;
	
	/* Setup the struct */
	result.w_block = w_block_count;
	result.h_block = h_block_count;
	
	/* Let's allocate the matrix and fill with zeros */
	result.blocks = calloc(h_block_count, sizeof(pixel_block*));
	int i;
	for(i=0; i<h_block_count; i++) 
		result.blocks[i] = calloc(w_block_count, sizeof(pixel_block));
    
	return result;
}

/* Applies the quantization using a typical quantization matrix, 
   as specified in the original JPEG Standard
 */
void do_quantization(value_blocks vb) {
    int i, j, line, column;
    /* A typical quantization matrix, as specified in the original JPEG Standard */
    quantizationMatrix qMatrix = {{16, 11, 10, 16, 24,  40,  51,  61 }, 
                                  {12, 12, 14, 19, 26,  58,  60,  55 },
                                  {14, 13, 16, 24, 40,  57,  69,  56 },
                                  {14, 17, 22, 29, 51,  87,  80,  62 },
                                  {18, 22, 37, 56, 68,  109, 103, 77 }, 
                                  {24, 35, 55, 64, 81,  104, 113, 92 },
                                  {49, 64, 78, 87, 103, 121, 120, 101},
                                  {72, 92, 95, 98, 112, 100, 103, 99 }};
    
    intValue_blocks result = create_intValue_blocks(vb.w_block, vb.h_block);
    
    printf("\nQuantization...\n");
    /* Runs all the 8x8 blocks */
    for (line = 0; line < result.h_block; line++) {
        for (column = 0; column < result.w_block; column++) {
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    result.blocks[line][column].data[i][j].red = (int)round(vb.blocks[line][column].data[i][j].red / qMatrix[i][j]);
                    result.blocks[line][column].data[i][j].green = (int)round(vb.blocks[line][column].data[i][j].green / qMatrix[i][j]);
                    result.blocks[line][column].data[i][j].blue = (int)round(vb.blocks[line][column].data[i][j].blue / qMatrix[i][j]);
                }
            }
        }
    }
    /* Prints one of the results for analysis */
    printf("\nQuantization results for the first block (red component)\n");
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            printf("%d ", result.blocks[0][0].data[i][j].red);
        }
        printf("\n");
    }
}

/* Undo the quantization
 */
void undo_quantization(intValue_blocks intvb) {
    int i, j, line, column;
    /* A typical quantization matrix, as specified in the original JPEG Standard */
    quantizationMatrix qMatrix = {{16, 11, 10, 16, 24,  40,  51,  61 }, 
                                  {12, 12, 14, 19, 26,  58,  60,  55 },
                                  {14, 13, 16, 24, 40,  57,  69,  56 },
                                  {14, 17, 22, 29, 51,  87,  80,  62 },
                                  {18, 22, 37, 56, 68,  109, 103, 77 }, 
                                  {24, 35, 55, 64, 81,  104, 113, 92 },
                                  {49, 64, 78, 87, 103, 121, 120, 101},
                                  {72, 92, 95, 98, 112, 100, 103, 99 }};
    
    value_blocks result;
    
    result.w_block = intvb.w_block;
	result.h_block = intvb.h_block;
	
	/* Let's allocate the matrix and fill with zeros */
	result.blocks = calloc(intvb.h_block, sizeof(block*));
	int k;
	for(k=0; k<intvb.h_block; k++) 
		result.blocks[i] = calloc(intvb.w_block, sizeof(block));
    
    printf("\nUndo the Quantization...\n");
    /* Runs all the 8x8 blocks */
    for (line = 0; line < result.h_block; line++) {
        for (column = 0; column < result.w_block; column) {
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    result.blocks[line][column].data[i][j].red = (float)round(intvb.blocks[line][column].data[i][j].red * qMatrix[i][j]);
                    result.blocks[line][column].data[i][j].green = (float)round(intvb.blocks[line][column].data[i][j].green * qMatrix[i][j]);
                    result.blocks[line][column].data[i][j].blue = (float)round(intvb.blocks[line][column].data[i][j].blue * qMatrix[i][j]);
                }
            }
        }
    }
    /* Prints one of the results for analysis */
    printf("\nResults after undo the quantization for the first block (red component)\n");
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            printf("%f ", result.blocks[0][0].data[i][j].red);
        }
        printf("\n");
    }
}


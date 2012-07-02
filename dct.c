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
value_blocks create_value_blocks(int w_block_count, int h_block_count) {
	value_blocks result;
	
	/* Setup the struct */
	result.w_block = w_block_count;
	result.h_block = h_block_count;
	
	/* Let's allocate the matrix and fill with zeros */
	result.blocks = calloc(h_block_count, sizeof(block*));
	int i;
	for(i=0; i<h_block_count; i++) 
		result.blocks[i] = calloc(w_block_count, sizeof(block));
    
	return result;
}

/* Each 8×8 block of each component (R, G, B) is converted to a frequency-domain 
   representation, using a discrete cosine transform (DCT). 
 */
void dct(pixel_blocks pb) {
    int i, j, x, y, line, column;
    float ci, cj, sumR, sumG, sumB;
    ci = 1;
    cj = 1;
    sumR = 0;
    sumG = 0;
    sumB = 0;
    
    value_blocks result = create_value_blocks(pb.w_block_count, pb.h_block_count);
    
    printf("\n\nDCT...\n");
    /* Runs all the 8x8 blocks */
    for (line = 0; line < result.h_block; line++) {
        for (column = 0; column < result.w_block; column++) {
            for (i = 0; i < 8; i++) {
                if (i == 0) {
                    ci = (1/sqrt(2));
                }
                for (j = 0; j < 8; j++) {
                    if (j == 0) {
                        cj = (1/sqrt(2));
                    }
                    for (x = 0; x < 8; x++) {
                        for (y = 0; y < 8; y++) {
                            /* Computes the sum for each RGB component */
                            sumR += pb.blocks[line][column].data[x][y].red*cos(((2*x +1)*i*M_PI)/16)*cos(((2*y +1)*j*M_PI)/16);
                            sumG += pb.blocks[line][column].data[x][y].green*cos(((2*x +1)*i*M_PI)/16)*cos(((2*y +1)*j*M_PI)/16);
                            sumB += pb.blocks[line][column].data[x][y].blue*cos(((2*x +1)*i*M_PI)/16)*cos(((2*y +1)*j*M_PI)/16);
                        }
                    }
                    /* Stores the final value of the DCT */
                    result.blocks[line][column].data[i][j].red = 0.25*ci*cj*sumR;
                    result.blocks[line][column].data[i][j].green = 0.25*ci*cj*sumG;
                    result.blocks[line][column].data[i][j].blue = 0.25*ci*cj*sumB;
                    sumR = 0;
                    sumG = 0;
                    sumB = 0;
                    cj = 1;
                }
                ci = 1;
            }
        }
    }
    /* Prints one of the results for analysis */
    printf("\nDCT results for the first block (red component)\n");
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            printf("%f ", result.blocks[0][0].data[i][j].red);
        }
        printf("\n");
    }
    do_quantization(result);
}

/* Applies the inverse DCT (IDCT)
 */
void idct(value_blocks vb) {
    int i, j, x, y, line, column;
    float ci, cj, sumR, sumG, sumB;
    ci = 1;
    cj = 1;
    sumR = 0;
    sumG = 0;
    sumB = 0;
    
    value_blocks result = create_value_blocks(vb.w_block, vb.h_block);
    
    printf("\nIDCT...\n");
    /* Runs all the 8x8 blocks */
    for (line = 0; line < result.h_block; line++) {
        for (column = 0; column < result.w_block; column++) {
            for (x = 0; x < 8; x++) {
                for (y = 0; y < 8; y++) {
                    for (i = 0; i < 8; i++) {
                        if (i == 0) {
                            ci = (1/sqrt(2));
                        }
                        for (j = 0; j < 8; j++) {
                            if (j == 0) {
                                cj = (1/sqrt(2));
                            }
                            /* Computes the sum for each RGB component */
                            sumR += ci*cj*vb.blocks[line][column].data[i][j].red*cos(((2*x +1)*i*M_PI)/16)*cos(((2*y +1)*j*M_PI)/16);
                            sumG += ci*cj*vb.blocks[line][column].data[i][j].green*cos(((2*x +1)*i*M_PI)/16)*cos(((2*y +1)*j*M_PI)/16);
                            sumB += ci*cj*vb.blocks[line][column].data[i][j].blue*cos(((2*x +1)*i*M_PI)/16)*cos(((2*y +1)*j*M_PI)/16);
                            cj = 1;
                        }
                        ci = 1;
                    }
                    /* Stores the final value of the IDCT */
                    result.blocks[line][column].data[x][y].red = 0.25*sumR;
                    result.blocks[line][column].data[x][y].green = 0.25*sumG;
                    result.blocks[line][column].data[x][y].blue = 0.25*sumB;
                    sumR = 0;
                    sumG = 0;
                    sumB = 0;
                }
            }
        }
    }
    /* Prints one of the results for analysis */
    printf("\nIDCT results for the first block (red component)\n");
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            printf("%f ", result.blocks[0][0].data[i][j].red);
        }
        printf("\n");
    }
}
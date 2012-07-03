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


/* Applies the quantization using a typical quantization matrix, 
   as specified in the original JPEG Standard
 */
pixel_blocks do_quantization(value_blocks vb) {
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
    
    pixel_blocks result = create_pixel_blocks(vb.w_block, vb.h_block);
    
    /* Runs all the 8x8 blocks */
    for (line = 0; line < result.h_block_count; line++) {
        for (column = 0; column < result.w_block_count; column++) {
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
    //printf("\nQuantization results for the first block (red component)\n");
    //for (i = 0; i < 8; i++) {
    //    for (j = 0; j < 8; j++) {
    //        printf("%d ", result.blocks[0][0].data[i][j].red);
    //    }
    //    printf("\n");
    //}

    return result;
}

/* Undo the quantization
 */
value_blocks undo_quantization(pixel_blocks pb) {
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
    
    value_blocks result = create_value_blocks(pb.w_block_count, pb.h_block_count);
    
    /* Runs all the 8x8 blocks */

    for (line = 0; line < result.h_block; line++) {
        for (column = 0; column < result.w_block; column++) {
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    result.blocks[line][column].data[i][j].red = (float)round(pb.blocks[line][column].data[i][j].red * qMatrix[i][j]);
                    result.blocks[line][column].data[i][j].green = (float)round(pb.blocks[line][column].data[i][j].green * qMatrix[i][j]);
                    result.blocks[line][column].data[i][j].blue = (float)round(pb.blocks[line][column].data[i][j].blue * qMatrix[i][j]);
                }
            }
        }
    }
    /* Prints one of the results for analysis */
    //printf("\nResults after undoing the quantization for the first block (red component)\n");
    //for (i = 0; i < 8; i++) {
    //    for (j = 0; j < 8; j++) {
    //        printf("%f ", result.blocks[0][0].data[i][j].red);
    //    }
    //    printf("\n");
    //}
    return result;
}


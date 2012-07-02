/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

typedef int quantizationMatrix[8][8];

pixel_blocks do_quantization(value_blocks vb);
value_blocks undo_quantization(pixel_blocks pb);

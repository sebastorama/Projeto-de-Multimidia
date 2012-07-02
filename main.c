/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "block_division.h"
#include "zigzagscan.h"
#include "rle.h"
#include "dct.h"
#include "quantization.h"

void print_usage() {
	printf("Usage:\n   compressor <file>\n  Where file is a windows bmp file");
}

/* Main: parameters are
	input_file - path to the bitmap file that will be processed
	Nothing more, sorry to disapoint you ;)
	*/
int main(int argc, char * argv[]) {
	FILE *input_file;
	FILE *decompressed_file;

	input_file = fopen(argv[1], "rb");
	if(argc < 2) {
		print_usage();
		exit(0);
	}
	
	if (input_file == NULL) {
		printf("Arquivo de imagem não encontrado");
		return -1;
	}
	
	/* Extract the bitmap header from the input */
	bmp_header header = get_header(input_file);
	/* Extract the pixel data from the input */
	BMP_pixel_matrix bmp_matrix = get_pixels(input_file);
	
	/* Print the parsed header */
	print_header(header);
	/* Organize the original pixel matrix on the 8x8 blocks */
	printf("\n\n\nBreaking the original matrix into 8x8 blocks");
	pixel_blocks pb = BMP_pixel_matrix_2_pixel_blocks(bmp_matrix);
	
	/* Save dimensions for further usage */
	int w_block_count = pb.w_block_count;
	int h_block_count = pb.h_block_count;
	int blocks_count = w_block_count*h_block_count;

	/* Compression stage */
    
	value_blocks vb = dct(pb);
	pixel_blocks quantized_pb = do_quantization(vb);

	/* Do zigzag scan on blocks */
	printf("\nZigzagscanning ...");
	vector * vectors = 
		(vector *)do_zigzagscan(quantized_pb);

	printf("\nDC encoding ...");
	/* Difference coding on the DC coef */
	do_dc(vectors, blocks_count); 

	/* Run Lenght coding on the remaining */
	printf("\nRun lenght encoding ...");
	run_length_vector * rl_vectors =
		(run_length_vector *) do_rl(vectors, blocks_count);

	/* Decompression stage */

	/* Decode the run length */

	printf("\n\n\nNow decoding...Decoding run lenght ...");
	vector * decompressed_vectors = 
		(vector *)undo_rl(rl_vectors, blocks_count);

	/* Decode de difference coding */
	printf("\ndecoding the difference encoding ...");
	undo_dc(decompressed_vectors, blocks_count);

	/* Reverse the vector to a block with zigzagscan inverse */
	printf("\nreverting the vectorization process...");
	pixel_blocks decompressed_pb =
		undo_zigzagscan(decompressed_vectors, w_block_count, h_block_count);

	value_blocks decompressed_vb = undo_quantization(decompressed_pb);
	decompressed_pb = idct(decompressed_vb);

	/* Join the 8x8 pixel blocks to one single matrix */
	printf("\njoining the 8x8 blocks into a single matrix");
	BMP_pixel_matrix decompressed_pm =
		pixel_blocks_2_BMP_pixel_matrix(decompressed_pb);
	
	/* Save an output file for testing, this is the same as the input, just to 
	test the flow of data */
	decompressed_file = fopen("decompressed.bmp", "w+b");
	write_header(header, decompressed_file);
	write_pixels(header, decompressed_pm, decompressed_file);
	fclose(decompressed_file);
	printf("\n\n == The results of the compression/decompression stage are in decompressed.bmp file");

	return 0;
}

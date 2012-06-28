/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

bmp_header get_header(FILE * f) {
	bmp_header result;
	fseek(f, 0, SEEK_SET); 
	fread(&result, sizeof(bmp_header), 1, f);
	return result;
}

void write_header(bmp_header header, FILE *f) {
	fseek(f, 0, SEEK_SET);
	fwrite(&header , sizeof(bmp_header), 1, f);
}

void int_TO_LE(int value, int size, char * result) {
	int i;
	for(i=0; i<size; i++) {
		value = value >> i*8;      /* Position the portion to be extracted in
                                      the less significant byte */
		result[i] = value & 0xFF;  /* Extract the less significant byte */
	}
}

int LE_TO_int(unsigned char * data, int size)
{
	int result = 0;
	int i;
	int aux;
	for(i=0; i<size; i++) {
		aux = 0;                 /* fill aux with 0x00000000 */
		aux += data[i];          /* put the data on the less significant byte */
		result += aux << (i*8);  /* rotate the aux untill the byte is in the 
		                            correct position, than put in the result */
	}
	return result;
}


BMP_pixel_matrix init_BMP_pixel_matrix(BMP_pixel ** pixels, int height, int width) {
	BMP_pixel_matrix result;
	result.pixels = pixels;
	result.height = height;
	result.width = width;

	return result;
}


BMP_pixel_matrix get_pixels(FILE * f) {
    /* Extract the Windows Bitmap Header */
	bmp_header header = get_header(f);
	
	/* Extract relevant data from the Windows Bitmap Header */
	int bitmap_offset = LE_TO_int(header.bitmap_offset, 4);
	int height = LE_TO_int(header.height, 4);
	int width = LE_TO_int(header.width, 4);
	
	/* Alloc pointers to hold the pixel rows */
	BMP_pixel ** pixels = calloc(height, sizeof(BMP_pixel*));
	
	fseek(f, bitmap_offset, SEEK_SET);
	int i;
	for(i=0; i<height; i++) {   /* For each row */
		pixels[i] = calloc(width, sizeof(BMP_pixel));  /* Alloc an array of BMP_pixel */
		fread(pixels[i], sizeof(BMP_pixel)*width, 1, f); /* Copy from file to my array */
	}
	
	return init_BMP_pixel_matrix(pixels, height, width);
	
}

void write_pixels(bmp_header header, BMP_pixel_matrix m, FILE * f) {
     fseek(f, 0, SEEK_END);
     int i,j;
     
     /* Just run throught the pixel matrix and write the pixels one by one */
     for(i=0; i<m.height; i++)
              for(j=0; j<m.width; j++)
                       fwrite(&m.pixels[i][j], sizeof(BMP_pixel), 1, f);
}

/* Helper function to print a field of the header in it's raw form */
print_field_raw(char * name, unsigned char * value, int size) {
	int i;
	printf("%s (raw data): ", name);
	for(i=0; i<size; i++) {
		printf("%x ", value[i]);
	}
	printf("\n");
}

/* Helper function to print a field of the header in a human readable form */
print_field_human_readable(char * name, unsigned char * value, int size) {
	int i;
	printf("%s (human readable data): ", name);
	printf("%d ", LE_TO_int(value, size));
	printf("\n");
}

void print_header(bmp_header header) { 
	printf("=== Header info:\n\n");
	print_field_raw("type", header.type, 2); 
	print_field_human_readable("size", header.size, 4); 
	print_field_raw("reserved", header.reserved, 4); 
	print_field_raw("bitmap_offset", header.bitmap_offset, 4);
	print_field_human_readable("header_size", header.header_size, 4); 
	print_field_raw("width", header.width, 4); 
	print_field_human_readable("height", header.height, 4); 
	print_field_human_readable("planes", header.planes, 2); 
	print_field_human_readable("bits_per_pixel", header.bits_per_pixel, 2); 
	print_field_raw("compression", header.compression, 4); 
	print_field_human_readable("bitmap_size", header.bitmap_size, 4);
	print_field_raw("horizontal_resolution", header.horizontal_resolution, 4);
	print_field_raw("vertical_resolution", header.vertical_resolution, 4);
	print_field_raw("num_colors", header.num_colors, 4); 
	print_field_raw("num_important_colors", header.num_important_colors, 4); 
}

/* Helper function to print a pixel */
void print_pixel(BMP_pixel p) {
	printf("%X %X %X-", p.green, p.red, p.blue);
}

void print_pixel_matrix(BMP_pixel_matrix matrix) {
	int i, j;
	for(i=0; i<matrix.height; i++) {
		for(j=0; j<matrix.width; j++) {
		print_pixel(matrix.pixels[i][j]);
		}
		printf(" row %d ==== \n\n", i);
	}
}

/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

/* Struct that hold the header of a Windows Bitmap file. Store everything
   with the same endianess and in the same order that you'll find on a bitmap
   file, you can read the first 54 bytes of the file and write them directly to this
   without having any issue */
typedef struct bmp_header {
	unsigned char type[2]; 
	unsigned char size[4]; 
	unsigned char reserved[4]; 
	unsigned char bitmap_offset[4];
	unsigned char header_size[4]; 
	unsigned char width[4]; 
	unsigned char height[4]; 
	unsigned char planes[2]; 
	unsigned char bits_per_pixel[2]; 
	unsigned char compression[4]; 
	unsigned char bitmap_size[4];
	unsigned char horizontal_resolution[4];
	unsigned char vertical_resolution[4];
	unsigned char num_colors[4]; 
	unsigned char num_important_colors[4]; 
} bmp_header;

/* Struct that store the 24bit pixel. The byte order is the same that you'll
   find in the bitmap file */
typedef struct BMP_pixel {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} BMP_pixel;

/* Struct that store the pixel matrix of the bitmap */
typedef struct BMP_pixel_matrix {
	int width;       /* Width size */
	int height;      /* Height size */
	BMP_pixel ** pixels;  /* Hold the data */
} BMP_pixel_matrix;

/* This give you a Windows Bitmap Header given width, height, bits per pixel */
bmp_header build_header(int width, int height, int bpp);

/* Extract the header from a Windows Bitmap file */
bmp_header get_header(FILE * f);

/* Write the Windows Bitmap header into the beginning of the file. */
void write_header(bmp_header header, FILE * f);

/* Extract the pixel data from a Windows Bitmap file */
BMP_pixel_matrix get_pixels(FILE * f);

/* Write the pixel data on the parameter 'm'  on the file 'f'. The offset
   of this data inside the will have to be properly set on the 'header'
   parameter.
   WARNING: this is writing the pixels at the end of the file. For some reason
   the bitmap_offset inside header is not working. I'll retain the interface for
   further corrections */
void write_pixels(bmp_header header, BMP_pixel_matrix m, FILE * f);

/* Convert a integer value to a little endian sequence of bytes. Parameters are:
   'value' - Integer variable that holds the value to be converted
   'size'  - The size in bytes of your representation (max 4 or 8 on x86_64)
   'result' - the resulting byte sequence
   */
void int_TO_LE(int value, int size, char * result);

/* Convert a little endian byte sequence contained in 'data'  parameter, wich
   measure 'size' bytes into a "big endian" integer and return the value. The
   maximum size is arch dependent (4 on x86, 8 on x86_64, and so on...)*/
int LE_TO_int(unsigned char * data, int size);

/* Pretty print of the Windows Bitmap header structure. Some values are
   converted from raw little-endian bytes to actual human readable numbers
   for convenience, and some are just printed in it's pure form. Don't worry,
   the output will warn you which is which */
void print_header(bmp_header header);

/* Print the pixel data from a BMP_pixel_matrix. Usefull for debug purposes
   only */
void print_pixel_matrix(BMP_pixel_matrix data);

BMP_pixel_matrix init_BMP_pixel_matrix(BMP_pixel ** pixels, int height, int width);

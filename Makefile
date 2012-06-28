all: 
	gcc block_division.c dc.c bmp.c zigzagscan.c rle.c main.c -o compressor -g -m32

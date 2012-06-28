/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "block_division.h"
#include "zigzagscan.h"
#include "dc.h"

void do_dc(vector * v, int count_vector){
	int i;
	/* run through all DC cohefficients encoding
	 * the difference of adjacents
	 */
	for (i = count_vector-1; i > 0; i--) {
		v[i].data[0].red -= v[i-1].data[0].red;
		v[i].data[0].green -= v[i-1].data[0].green;
		v[i].data[0].blue -= v[i-1].data[0].blue;
	}
}

void undo_dc(vector *v, int count_vector) {
	int i;
	/* run through all DC cohefficients decoding
	 * the differences between adjacents
	 */
	for (i = 1; i < count_vector; i++) {
		v[i].data[0].red += v[i-1].data[0].red;
		v[i].data[0].green += v[i-1].data[0].green;
		v[i].data[0].blue += v[i-1].data[0].blue;
	}
}

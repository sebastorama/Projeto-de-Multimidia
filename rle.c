/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

#include <stdlib.h>
#include <stdio.h>

#include "bmp.h"
#include "block_division.h"
#include "zigzagscan.h"
#include "rle.h"

/* The next two functions are for modifying the underlying data structure.
 * Originally we had vector[i].data[j].red, vector[i].data[j].green,
 * and so on. I should be able to do red[i] or green[i] directly for
 * the rl structure. The problem is that each component can vary in
 * size, for example, the red can be 62 sized and the green 53 on the
 * run lenght encoded result, so they should be treated separately
 */
void join_components(vector * v, unsigned char * green, unsigned char * red, unsigned char * blue) {
	int i;
	for(i=0; i<VECTOR_SIZE; i++) {
		v->data[i].green = green[i];
		v->data[i].red = red[i];
		v->data[i].blue = blue[i];
	}

}

void extract_components(vector v, unsigned char * green, unsigned char * red, unsigned char * blue) {
	int i;
	for(i=0; i<VECTOR_SIZE; i++) {
		green[i] = v.data[i].green;
		red[i] = v.data[i].red;
		blue[i] = v.data[i].blue;
	}
}

/* Auxiliar funcion on the rl encoding, just searches for the next
 * non zero component and returns its index */
int next_non_zero_index(unsigned char * component, int index) {
	int i;
	for(i=index+1; i<VECTOR_SIZE; i++) 
		if(component[i] != 0) return i;
	return -1;
}

void do_rl_in_component(unsigned char * component, run_length_component * rl_component, int * rl_component_size) {	
	*rl_component_size = 0; 
	int current_index = 0;
	int nnz = 0;

	/* While the next non zero index is inside our vector */
	while(next_non_zero_index(component, current_index) != -1) {
		/* Get the next non zero index */
		nnz = next_non_zero_index(component, current_index);
		/* Calculate the skip part, basically:
		 * the index of the next non-zero element minus the current element minus one
		 */
		rl_component[*rl_component_size].skip = (unsigned char)(nnz - current_index - 1);
		/* The value of the next non-zero element found */
		rl_component[*rl_component_size].value = component[nnz];
		/* Just added one rl_component, count it up! */
		*rl_component_size = *rl_component_size + 1;

		/* Do the calculation again for the non-zero element found */
		current_index = nnz;
	}

	/* If eventually we didnt found non-zero elements on the rest of
	 * the vector, the termination code is inserted 
	 */
	if(current_index < VECTOR_SIZE-1) { 
		rl_component[*rl_component_size].skip = 0;
		rl_component[*rl_component_size].value = 0;
	}
}

/* Convert a serialized vector into a run_lenght_vector */
run_length_vector vector_to_rlv(vector v) {
	/* Allocate the components */
	unsigned char * green_vector = calloc(VECTOR_SIZE, sizeof(char));
	unsigned char * red_vector = calloc(VECTOR_SIZE, sizeof(char));
	unsigned char * blue_vector = calloc(VECTOR_SIZE, sizeof(char));

	/* Allocate my run_length_vector */
	run_length_vector result;
	/* Extract red, green and blue components from the original vector */
	extract_components(v, green_vector, red_vector, blue_vector);

	/* Do the run lenght encoding on each of them */
	do_rl_in_component(green_vector, result.green, &result.size_green);
	do_rl_in_component(red_vector, result.red, &result.size_red);
	do_rl_in_component(blue_vector, result.blue, &result.size_blue);

	/* Copy the cd cohefficient */
	result.green_dc = green_vector[0];
	result.red_dc = red_vector[0];
	result.blue_dc = blue_vector[0];

	return result;
}


run_length_vector* do_rl(vector* v, int vector_count) {
	run_length_vector * result = (run_length_vector *)calloc(vector_count, sizeof(run_length_vector));
	int i;

	/* Run through all the vectors encoding them */
	for(i=0; i<vector_count; i++) {
		result[i] = vector_to_rlv(v[i]); 
	}
	return result;
}

unsigned char * undo_rl_in_component(run_length_component * component, int size) {
	/* Alloc our response and FILL WITH ZEROS,
	 * seriously, the fill with zeros part is important
	 */
	unsigned char * result = (unsigned char *)calloc(VECTOR_SIZE, sizeof(unsigned char));

	int i;
	int result_size = 1;
	/* For each run_length_component, */
	for(i=0; i<size; i++)
		/* Check if it has the termination code */
		if(component[i].value != 0) {
			/* Just skip what we were told to, remember
			 * that very important zero part above? ;)
			 */
			result_size += component[i].skip;

			/* Set the value when done skipping */
			result[result_size] = component[i].value;

			/* Point to the next value */
			result_size++;
		} else break;

	return result;
}

vector rlv_to_vector(run_length_vector rlv) {
	vector result;

	/* We need to decode each component separately */
	unsigned char * green_component =
		undo_rl_in_component(rlv.green, rlv.size_green);
	unsigned char * red_component   =
		undo_rl_in_component(rlv.red, rlv.size_red);
	unsigned char * blue_component  =
		undo_rl_in_component(rlv.blue, rlv.size_blue);

	/* Then copy the DC cohefficients back on each component */
	red_component[0] = rlv.red_dc;
	green_component[0] = rlv.green_dc;
	blue_component[0] = rlv.blue_dc;

	/* Join all the components back to do v.data[i].green, as explained above */
	join_components(&result, green_component, red_component, blue_component);

	return result;
}

vector * undo_rl(run_length_vector * rlv, int rlv_count) {
	int i;
	vector * result = (vector *)calloc(rlv_count, sizeof(vector));
	/* For each run_length_vector, convert it to a normal serialized vector */
	for(i=0; i<rlv_count; i++) {
		result[i] = rlv_to_vector(rlv[i]);
	}

	return result;
}

/* Writes the compressed file on disk */
void write_compressed_file(int w_block_count, int h_block_count, run_length_vector * rl_vectors, FILE *f) {
	int i, k, block_count;
    block_count = w_block_count * h_block_count;
    
    /* Writes the dimensions */
    fwrite(&w_block_count , sizeof(int), 1, f);
    fwrite(&h_block_count , sizeof(int), 1, f);
    
    /* Writes the information of all the blocks */
    for (i = 0; i < block_count; i++) {
        fwrite(&rl_vectors[i].size_red, sizeof(int), 1, f);
        fwrite(&rl_vectors[i].size_green, sizeof(int), 1, f);
        fwrite(&rl_vectors[i].size_blue, sizeof(int), 1, f);
        fwrite(&rl_vectors[i].red_dc, sizeof(int), 1, f);
        fwrite(&rl_vectors[i].green_dc, sizeof(int), 1, f);
        fwrite(&rl_vectors[i].blue_dc, sizeof(int), 1, f);
        
        for (k = 0; k < rl_vectors[i].size_red; k++) {
            fwrite(&rl_vectors[i].red[k].skip, sizeof(unsigned char), 1, f);
            fwrite(&rl_vectors[i].red[k].value, sizeof(unsigned char), 1, f);
        }
        for (k = 0; k < rl_vectors[i].size_green; k++) {
            fwrite(&rl_vectors[i].green[k].skip, sizeof(unsigned char), 1, f);
            fwrite(&rl_vectors[i].green[k].value, sizeof(unsigned char), 1, f);
        }
        for (k = 0; k < rl_vectors[i].size_blue; k++) {
            fwrite(&rl_vectors[i].blue[k].skip, sizeof(unsigned char), 1, f);
            fwrite(&rl_vectors[i].blue[k].value, sizeof(unsigned char), 1, f);
        }
    }
}

/* Reads the compressed file from disk */
run_length_vector* read_compressed_file(int* w_block_count, int* h_block_count, FILE *f) {
    int i, k, w_count, h_count;
    
    /* Reads the dimensions */
    fread(&w_count , sizeof(int), 1, f);
    fread(&h_count , sizeof(int), 1, f);
    
    *w_block_count = w_count;
    *h_block_count = h_count;
    
    run_length_vector * result = (run_length_vector *)calloc(w_count*h_count, sizeof(run_length_vector));
    
    /* Reads the information of all the blocks */
    for (i = 0; i < w_count*h_count; i++) {
        fread(&result[i].size_red, sizeof(int), 1, f);
        fread(&result[i].size_green, sizeof(int), 1, f);
        fread(&result[i].size_blue, sizeof(int), 1, f);
        fread(&result[i].red_dc, sizeof(int), 1, f);
        fread(&result[i].green_dc, sizeof(int), 1, f);
        fread(&result[i].blue_dc, sizeof(int), 1, f);
        
        for (k = 0; k < result[i].size_red; k++) {
            fread(&result[i].red[k].skip, sizeof(unsigned char), 1, f);
            fread(&result[i].red[k].value, sizeof(unsigned char), 1, f);
        }
        for (k = 0; k < result[i].size_green; k++) {
            fread(&result[i].green[k].skip, sizeof(unsigned char), 1, f);
            fread(&result[i].green[k].value, sizeof(unsigned char), 1, f);
        }
        for (k = 0; k < result[i].size_blue; k++) {
            fread(&result[i].blue[k].skip, sizeof(unsigned char), 1, f);
            fread(&result[i].blue[k].value, sizeof(unsigned char), 1, f);
        }
    }
    return result;
}



/* 
 * Sebastião Giacheto Ferreira Júnior - 5634611
 * Willian Gustavo Pinch - 6426792
 */

#define VECTOR_SIZE 64

/* Used to store "skip" and "value"
 * on the run length encoding. 
 */
typedef struct {
	unsigned char value;
	unsigned char skip;
} run_length_component;

/* Structure that represent the data after
 * doing a run length encode on a serialized
 * vector
 */
typedef struct {
	/* Green skip/value component */
	run_length_component green[63];
	/* Red skip/value component */
	run_length_component red[63];
	/* Blue skip/value component */
	run_length_component blue[63];

	/* Each component have its own
	 * size. One can be smaller or
	 * greater than the other and
	 * should be treated differently
	 */
	int size_green;
	int size_red;
	int size_blue;

	/* The DC components of the original
	 * vector. In pratical terms, the first
	 * cohefficient of each component
	 */
	int green_dc;
	int red_dc;
	int blue_dc;
} run_length_vector;

/* Get a serialized vector, and applies the run length
 * encoding, returning it on the appropriated structure.
 * The DC components are copied over this new struct
 */
run_length_vector* do_rl(vector* v, int vector_count);

/* Gets a run_lenght encoded vector and convert into a 
 * serialized vector. The DC components saved in the
 * encoding process are recovered too
 */
vector * undo_rl(run_length_vector * rlv, int rlv_count);

/* Writes the compressed file on disk
 */
void write_compressed_file(int w_block_count, int h_block_count, run_length_vector * rl_vectors, FILE *f);

/* Reads the compressed file from disk 
 */
run_length_vector* read_compressed_file(int* w_block_count, int* h_block_count, FILE *f);


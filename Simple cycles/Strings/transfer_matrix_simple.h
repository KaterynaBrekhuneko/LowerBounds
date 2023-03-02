#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Share of signatures to be generated during one iteration
#define GROUP_SIZE  12

/*
 * Binary search function to search throgh the list of signatures.
 */
long long binary_search(char **valid_signatures, char *signature, long long l, long long r, int w);

/*
 * Finds an index of the signature in the list of valid signatures.
 */
long long find_index(char ***valid_signatures, char *signature, long long *counts, int w);

/*
 * Returns <true> if the signature is valid, <false> otherwise.
 */
bool is_valid_signature(char *signature, int w);

/*
 * Generates a <dimesion> quantity of signatures starting at index n.
 */
void construct_signatures(char **signatures, unsigned int dimension, int w, unsigned int n);
 
/*
 * Saves only valid signatures from the list of all signatures to the list of valid signatures.
 */
void save_valid_signatures(char **valid_signatures, char **signatures, unsigned int dimension, int w);
 
/*
 * Returns the amount of valid signatures in the list of signatures.
 */
unsigned int count_valid_signatures(char **signatures, unsigned int dimension, int w);
 
/*
 * Changes a charakter at index i to c.
 */
void change_char(char *signature, char *signature_copy, int i, char *c, int w);

/*
 * Changes the B state corresponding to the given A state to an A state.
 */
char* change_to_a(char *signature_copy, char *signature, int number_of_a, int w);

/*
 * Changes the A state corresponding to the given B state to a B state.
 */
char* change_to_b(char *signature_copy, char *signature, int number_of_b, int w);
   
/*
 * Returns <true> if the given A and B states are on the same path, <false> otherwise.
 */
bool are_on_the_same_path(char *signature, int index_a, int offset_b);
   
/*
 * Computes a list of successor signatures for a given signature.
 * Returns the length of the final list.
 */
int compute_successor_signatures(char **next_signatures, char *signature, int w);
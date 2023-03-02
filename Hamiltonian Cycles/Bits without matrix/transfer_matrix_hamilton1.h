#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//Share of signatures to be generated during one iteration
#define GROUP_SIZE  12

/*
 * Returns the string representation of a signature using the states A, B, O, X.
 */
void get_string_rep(unsigned long long signature, int w);

/*
 * Binary search function to search throgh the list of signatures.
 */
long long binary_search(unsigned long long *valid_signatures, unsigned long long signature, long long l, long long r, int w);

/*
 * Finds an index of the signature in the list of valid signatures.
 */
long long find_index(unsigned long long **valid_signatures, unsigned long long signature, long long *counts, int w);

/*
 * Returns <true> if the signature is valid, <false> otherwise.
 */
bool is_valid_signature(unsigned long long signature, int w);

/*
 * Generates a <dimesion> quantity of signatures starting at an index n.
 */
void construct_signatures(unsigned long long *signatures, unsigned long long dimension, int w, unsigned long long n);
 
/*
 * Saves only valid signatures from the list of all signatures to the list of valid signatures.
 */
void save_valid_signatures(unsigned long long *valid_signatures, unsigned long long *signatures, unsigned long long dimension, int w);
 
/*
 * Returns the amount of valid signatures in the list of signatures.
 */
long long count_valid_signatures(unsigned long long *signatures, unsigned long long dimension, int w);

/*
 * Changes the B state corresponding to the given A state to an A state.
 */
unsigned long long change_to_a(unsigned long long signature, int number_of_a, int w);

/*
 * Changes the A state corresponding to the given B state to a B state.
 */
unsigned long long change_to_b(unsigned long long signature, int number_of_b, int w);
   
/*
 * Returns <true> if the given A and B states are on the same path, <false> otherwise.
 */
bool are_on_the_same_path(unsigned long long signature, int index_a, int offset_b, int w);
   
/*
 * Computes a list of successor signatures for a given signature.
 * Returns the length of the final list.
 */
int compute_successor_signatures(unsigned long long *next_signatures, unsigned long long signature, int w);
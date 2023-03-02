#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
 * Returns the number of 2-Motzkin paths of length w.
 */
unsigned long long number_of_motzkin(int w);

/*
 * Constructs the structure S that is used to generate 2-Motzkin paths.
 */
void construct_S(unsigned long long **S, int w);

/*
 * Converts a 2-Motzkin path to an integer from 1 to M_n.
 */
unsigned long long rank(unsigned long long path, unsigned long long **S, int w);

/*
 * Converts an integer from 1 to M_n to a 2-Motzkin path.
 */
unsigned long long unrank(unsigned long long n, unsigned long long **S, int w);

/*
 * Returns the string representation of a signature using the states A, B, O, X.
 */
void get_string_rep(unsigned long long signature, int w);

/*
 * Returns <true> if the signature is valid, <false> otherwise.
 */
bool is_valid_signature(unsigned long long signature, int w);

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
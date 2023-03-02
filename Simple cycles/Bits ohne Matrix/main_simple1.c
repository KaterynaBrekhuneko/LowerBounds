#include "transfer_matrix_simple1.h"
#include "matrix_simple1.h"

unsigned long long generate_all_signatures(unsigned long long dimension, int w, unsigned long long *all_signatures, unsigned long long **valid_signatures, long long *counts);
void approximate_eigenvalue(unsigned long long dimension, unsigned long long **valid_signatures, long long *counts, int w);

int main() {

	//Width of the twisted cylinder
	int w;
	
	printf("Enter width: ");
	scanf("%d", &w);
	
	//Amount of currently valid signatures
    unsigned long long dimension = 9 * pow(4, w-1);
	
	//List of all generated signatures
    unsigned long long *all_signatures;

	//Holds <GROUP_SIZE> lists of valid signatures from each generated group of dignatures
	unsigned long long **valid_signatures = (unsigned long long**)malloc(GROUP_SIZE * sizeof(long long*));
	
	long long *counts = (long long*)malloc(GROUP_SIZE * sizeof(long long));	

    dimension = generate_all_signatures(dimension, w, all_signatures, valid_signatures, counts);
	
	printf("\nDimension: %u\n\n", dimension);
	
	approximate_eigenvalue(dimension, valid_signatures, counts, w);

    for(int i = 0; i<GROUP_SIZE; i++){
		free(valid_signatures[i]);
	}
	free(valid_signatures);
	free(counts);

    return 0;
}

/*
* Generates all signatures in groups of size dimension/GROUP_SIZE
* and saves valid signatures in the separate list.
*/
unsigned long long generate_all_signatures(unsigned long long dimension, int w, unsigned long long *all_signatures, unsigned long long **valid_signatures, long long *counts){
	long long count = 0;
	long long amount_of_valid_signatures = 0;
	
	for(int i = 0; i<GROUP_SIZE; i++){
		//Index of the first signature in the new group
		unsigned long long n = i*(dimension/GROUP_SIZE);

		all_signatures = (unsigned long long*)malloc((dimension/GROUP_SIZE) * sizeof(unsigned long long));
		construct_signatures(all_signatures, dimension, w, n);
		
		count = count_valid_signatures(all_signatures, dimension/GROUP_SIZE, w);
		
		valid_signatures[i] = (unsigned long long*)malloc(count * sizeof(unsigned long long));

		save_valid_signatures(valid_signatures[i], all_signatures, dimension/GROUP_SIZE, w);
		
		counts[i] = count;
		amount_of_valid_signatures += count;
		
		free(all_signatures);
	}
	
	return amount_of_valid_signatures;
}

/*
* Calculates the largest eigenvalue of the transfer matrix.
*/
void approximate_eigenvalue(unsigned long long dimension, unsigned long long **valid_signatures, long long *counts, int w){
	printf("Calculating eigenvalue...\n\n");
	
    double eigenvalue;
	double *eigenvector = (double*)malloc(dimension * sizeof(double));
	
	calculate_eigenvalue(dimension, valid_signatures, counts, w, &eigenvalue, eigenvector);
	
    printf("Eigenwert %f\n", eigenvalue);

    free(eigenvector);
}

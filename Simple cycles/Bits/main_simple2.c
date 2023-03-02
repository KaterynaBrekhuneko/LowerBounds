#include "transfer_matrix_simple2.h"
#include "matrix_simple2.h"

long long generate_all_signatures(unsigned long long dimension, int w, unsigned long long *all_signatures, unsigned long long **valid_signatures, long long *counts);
void fill_transfer_matrix(unsigned int **transfer_matrix, int w, unsigned long long **valid_signatures, long long *counts, unsigned short *lengths);
void approximate_eigenvalue(unsigned long long dimension, unsigned int **transfer_matrix, unsigned short *lengths);

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
	unsigned long long **valid_signatures = (unsigned long long**)malloc(GROUP_SIZE * sizeof(unsigned long long*));
	
	long long *counts = (long long*)malloc(GROUP_SIZE * sizeof(long long));	

    dimension = generate_all_signatures(dimension, w, all_signatures, valid_signatures, counts);
		
	unsigned short *lengths = (unsigned short *)malloc((dimension) * sizeof(unsigned short));
	unsigned int **transfer_matrix = (unsigned int**)malloc(dimension * sizeof(unsigned int*));
	
	printf("\nDimension: %lu\n\n", dimension);
	
	fill_transfer_matrix(transfer_matrix, w, valid_signatures, counts, lengths);
	
	for(int i = 0; i<GROUP_SIZE; i++){
		free(valid_signatures[i]);
	}
	free(valid_signatures);
	free(counts);
	
	approximate_eigenvalue(dimension, transfer_matrix, lengths);
	
	free(lengths);
	
	for (unsigned long long i = 0; i < dimension; i++){
        free(transfer_matrix[i]);
    }
    free(transfer_matrix);

    return 0;
}

/*
 * Generates all signatures in groups of size dimension/GROUP_SIZE
 * and saves valid signatures in the separate list.
 */
long long generate_all_signatures(unsigned long long dimension, int w, unsigned long long *all_signatures, unsigned long long **valid_signatures, long long *counts){
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
 * Computes a list of successors for every signature and 
 * saves the corresponding indiсes in the transfer matrix.
 */
void fill_transfer_matrix(unsigned int **transfer_matrix, int w, unsigned long long **valid_signatures, long long *counts, unsigned short *lengths){

	unsigned long long a = 0;
	long long signature_index = 0;
	
	for(int i = 0; i<GROUP_SIZE; i++){
		for(long long j = 0; j<counts[i]; j++){
			
			unsigned long long *successor_signatures = (unsigned long long*)malloc(4*sizeof(unsigned long long));

			//Amount of successor signatures of the current signature
			unsigned int length = compute_successor_signatures(successor_signatures, valid_signatures[i][j], w);
			
			transfer_matrix[a] =(unsigned int*)malloc((length + 1) * sizeof(unsigned int));
			lengths[a] = length;
			
			for(int b = 0; b<length; b++){
				
				signature_index = find_index(valid_signatures, successor_signatures[b], counts, w);

				transfer_matrix[a][b] = signature_index;
			}
			
			a++;
			
			free(successor_signatures); 
		}
	}
}

/*
 * Calculates the largest eigenvalue of the transfer matrix.
 */
void approximate_eigenvalue(unsigned long long dimension, unsigned int **transfer_matrix, unsigned short *lengths){
	printf("Calculating eigenvalue...\n\n");
	
    double eigenvalue;
	double *eigenvector = (double*)malloc(dimension * sizeof(double));
	
	calculate_eigenvalue(dimension, transfer_matrix, &eigenvalue, eigenvector, lengths);
	
    printf("Eigenwert %f\n", eigenvalue);

    free(eigenvector);
}

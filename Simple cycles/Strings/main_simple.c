#include "transfer_matrix_simple.h"
#include "matrix_simple.h"

int generate_all_signatures(unsigned int dimension, int w, char **all_signatures, char ***valid_signatures, long long *counts);
void fill_transfer_matrix(unsigned int **transfer_matrix, int w, char ***valid_signatures, long long *counts);
void approximate_eigenvalue(unsigned int dimension, unsigned int **transfer_matrix);

int main() {

	//Width of the twisted cylinder
	int w;
	
	printf("Enter width: ");
	scanf("%d", &w);
	
	//Amount of currently valid signatures
    unsigned int dimension = 9 * pow(4, w-1);
	
	//List of all generated signatures
    char **all_signatures;
	
	//Holds <GROUP_SIZE> lists of valid signatures from each generated group of dignatures
	char ***valid_signatures = (char***)malloc(GROUP_SIZE * sizeof(char**));
	
	//Contains information about how many signatures are in each list in *valid_signatures*
	long long *counts = (long long*)malloc(GROUP_SIZE * sizeof(long long));	

    dimension = generate_all_signatures(dimension, w, all_signatures, valid_signatures, counts);

    unsigned int **transfer_matrix = (unsigned int**)malloc(dimension * sizeof(unsigned int*));
	
	printf("\nDimension: %u\n\n", dimension);
	
	fill_transfer_matrix(transfer_matrix, w, valid_signatures, counts);
	
	for(int i = 0; i<GROUP_SIZE; i++){
		for(long long j = 0; j<counts[i]; j++){
			free(valid_signatures[i][j]);
		}
		free(valid_signatures[i]);
	}
	free(valid_signatures);
	free(counts);
	
	approximate_eigenvalue(dimension, transfer_matrix);

    for (unsigned int i = 0; i < dimension; i++){
        free(transfer_matrix[i]);
    }
    free(transfer_matrix);

    return 0;
}

/*
 * Generates all signatures in groups of size dimension/GROUP_SIZE
 * and saves valid signatures in the separate list.
 */
int generate_all_signatures(unsigned int dimension, int w, char **all_signatures, char ***valid_signatures, long long *counts){
	long long count = 0;
	long long amount_of_valid_signatures = 0;
	
	for(int i = 0; i<GROUP_SIZE; i++){
		//Index of the first signature in the new group
		unsigned int n = i*(dimension/GROUP_SIZE);

		all_signatures = (char**)malloc((dimension/GROUP_SIZE) * sizeof(char*));
		construct_signatures(all_signatures, dimension, w, n);
		
		count = count_valid_signatures(all_signatures, dimension/GROUP_SIZE, w);

		valid_signatures[i] = (char**)malloc(count * sizeof(char*));
		save_valid_signatures(valid_signatures[i], all_signatures, dimension/GROUP_SIZE, w);
		
		counts[i] = count;
		amount_of_valid_signatures += count;
		
		for(unsigned int j = 0; j<dimension/GROUP_SIZE; j++){
			free(all_signatures[j]);
		}
		free(all_signatures);
	}
	return amount_of_valid_signatures;
}

/*
 * Computes a list of successors for every signature and 
 * saves the corresponding indiсes in the transfer matrix.
 */
void fill_transfer_matrix(unsigned int **transfer_matrix, int w, char ***valid_signatures, long long *counts){
	unsigned int a = 0;
	int signature_index = 0;
	
	for(int i = 0; i<GROUP_SIZE; i++){
		for(long long j = 0; j<counts[i]; j++){
			
			char **successor_signatures = (char**)malloc(4*sizeof(char*));

			//Amount of successor signatures of the current signature
			int length = compute_successor_signatures(successor_signatures, valid_signatures[i][j], w);
			
			transfer_matrix[a] =(unsigned int*)malloc((length + 1) * sizeof(unsigned int));
			transfer_matrix[a][0] = length;
			
			for(int b = 1; b<length+1; b++){
				
				signature_index = find_index(valid_signatures, successor_signatures[b-1], counts, w);

				transfer_matrix[a][b] = signature_index;
			}
			
			a++;
			
			for(int b = 0; b<length; b++){
				free(successor_signatures[b]);
			}
			free(successor_signatures); 
		}
	}
}

/*
 * Calculates the largest eigenvalue of the transfer matrix.
 */
void approximate_eigenvalue(unsigned int dimension, unsigned int **transfer_matrix){
	printf("Calculating eigenvalue...\n\n");
	
    double eigenvalue;
    double *eigenvector = (double*)malloc(dimension * sizeof(double));

    calculate_eigenvalue(dimension, transfer_matrix, &eigenvalue, eigenvector);
    printf("Eigenwert %f\n", eigenvalue);

    free(eigenvector);
}
#include "transfer_matrix_hamilton3.h"
#include "matrix_hamilton3.h"

void fill_transfer_matrix(unsigned long long **transfer_matrix, int w, unsigned long long dimension, unsigned long long **S, unsigned short *lengths);
void approximate_eigenvalue(unsigned long long dimension, unsigned long long **transfer_matrix, unsigned short *lengths);

int main() {			
	//Width of the twisted cylinder
	int w;
	
	printf("Enter width: ");
	scanf("%d", &w);	
	
	//Amount of columns in S
	unsigned long long l = ((w+2)%2)==0 ? ((w+2)/2) : ((w+2)/2)+1;
	
	//Structure for generating 2-Motzkin paths
	unsigned long long **S = (unsigned long long **)malloc((w+2) * sizeof(unsigned long long *));
	//Allocates space for S and sets all values to 0
	for(int i = 0; i<w+2; i++){		
		S[i] = (unsigned long long *)malloc(l * sizeof(unsigned long long));
		
		for(unsigned long long j = 0; j<l; j++){
			S[i][j] = 0;
		}
	}
	
	construct_S(S, w+1);
	
	//Amount of valid signatures
	unsigned long long dimension = S[0][0];
	
	//Stores amounts of successors for each signature
	unsigned short *lengths = (unsigned short *)malloc((dimension) * sizeof(unsigned short));
	
	unsigned long long **transfer_matrix = (unsigned long long **)malloc(dimension * sizeof(unsigned long long *));
	fill_transfer_matrix(transfer_matrix, w, dimension, S, lengths);	
	
	for(int i = 0; i<w+2; i++){
		free(S[i]);
	}
	free(S);
	
	approximate_eigenvalue(dimension, transfer_matrix, lengths);
	
	free(lengths);
	
	for (unsigned long long i = 0; i < dimension; i++){
        free(transfer_matrix[i]);
    }
    free(transfer_matrix);
	
    return 0;
}

/*
 * Computes a list of successors for every signature and 
 * saves the corresponding indiсes in the transfer matrix.
 */
void fill_transfer_matrix(unsigned long long **transfer_matrix, int w, unsigned long long dimension, unsigned long long **S, unsigned short *lengths){

	unsigned long long signature_index = 0;
	
	for(unsigned long long i = 1; i<=dimension; i++){
		
		unsigned long long *successor_signatures = (unsigned long long *)malloc(4 * sizeof(unsigned long long));
		
		unsigned long long signature = unrank(i, S, w);

		int length = compute_successor_signatures(successor_signatures, signature, w);
		
		transfer_matrix[i-1] = (unsigned long long *)malloc((length) * sizeof(unsigned long long));
		lengths[i-1] = length;
		
		for(int j = 0; j<length; j++){			
			signature_index = rank(successor_signatures[j], S, w) - 1;

			transfer_matrix[i-1][j] = signature_index;
		}
		
		free(successor_signatures); 
	}	
}

/*
 * Calculates the largest eigenvalue of the transfer matrix.
 */
void approximate_eigenvalue(unsigned long long dimension, unsigned long long **transfer_matrix, unsigned short *lengths){
	printf("\nCalculating eigenvalue...\n\n");
	
    double eigenvalue;
	double *eigenvector = (double *)malloc(dimension * sizeof(double));
	
	calculate_eigenvalue(dimension, transfer_matrix, &eigenvalue, eigenvector, lengths);
	
    printf("Eigenwert %f\n", eigenvalue);

    free(eigenvector);
}
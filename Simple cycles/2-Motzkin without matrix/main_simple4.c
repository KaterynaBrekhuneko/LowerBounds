#include "transfer_matrix_simple4.h"
#include "matrix_simple4.h"

void approximate_eigenvalue(unsigned long long dimension, int w, unsigned long long **S);

int main() {

	//Width of the twisted cylinder
	int w;
	
	printf("Enter width: ");
	scanf("%d", &w);	

	unsigned long long l = ((w+2)%2)==0 ? ((w+2)/2) : ((w+2)/2)+1;
	
	unsigned long long **S = (unsigned long long **)malloc((w+2) * sizeof(unsigned long long *));
	for(int i = 0; i<w+2; i++){
		
		S[i] = (unsigned long long*)malloc(l * sizeof(unsigned long long));
		
		for(unsigned long long j = 0; j<l; j++){
			S[i][j] = 0;
		}
	}
	
	construct_S(S, w+1);
	
	//Amount of valid signatures
	unsigned long long dimension = S[0][0];
	
	approximate_eigenvalue(dimension, w, S);
		
	for(int i = 0; i<w+2; i++){
		free(S[i]);
	}
	free(S);
	
    return 0;
}

/*
 * Calculates the largest eigenvalue of the transfer matrix.
 */
void approximate_eigenvalue(unsigned long long dimension, int w, unsigned long long **S){
	printf("Calculating eigenvalue...\n\n");
	
    double eigenvalue;
	double *eigenvector = (double*)malloc(dimension * sizeof(double));
	
	calculate_eigenvalue(dimension, w, S, &eigenvalue, eigenvector);
	
    printf("Eigenwert %f\n", eigenvalue);

    free(eigenvector);
}

#include "transfer_matrix_hamilton1.h"
#include "matrix_hamilton1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int calculate_eigenvalue2(unsigned long long dimension, unsigned long long **valid_signatures, long long *counts, int width, double *eig, double *vec,
    unsigned long max_iter, double epsilon){
		
    unsigned int i = 0;
    double delta;
	double *tmp = (double *)malloc(sizeof(double) * dimension);


	/* Generate a random start vector with a big enough absolte value 
	 * in order to avoid rounding errors.*/
    do{ 
		v_init(dimension, vec); 
	}
	while(v_abs(dimension, vec) < 0.5);
    
	v_normalize(dimension, vec);

    do
    {
        /* Naechster Iterationsschritt */
        m_v_mul(dimension, tmp, valid_signatures, counts, vec, width);

        *eig = v_v_mul(dimension, tmp, vec);

        if(!*eig)
        {
			/* We chose the start vector perpendicular to the eigenvector 
			 * which corresponds to the largest eigenvalue.
			 * We generate another random start vector and start iterating again*/
            do{
				v_init(dimension, vec);
			}				
			while(v_abs(dimension, vec) < 0.5);
			
            v_normalize(dimension, vec);
			
			// Reset the error margin, so that the loop does not stop. 
            delta = 2.0 * epsilon; 
			i = 0;
        }
        else
        {		 
			/* For an eigenvalue e and its corresponding eigenvector it applies
			 * that |M * v - e * v| = 0. 
			 * We calculate the error margin delta = |M * v - e * v|.*/
            v_c_mul(dimension, vec, *eig);
            v_v_sub(dimension, tmp, vec);
            delta = v_abs(dimension, vec);

            /* Normalize the vector for the next iteration */
            v_copy(dimension, vec, tmp);
            v_normalize(dimension, vec);
        }
		
		i++;
    }
	// The iteration will stop when eather the suffisient error marginor the maximal number of iterations has been reached
    while((i < max_iter) && delta > epsilon);
	
    printf("%u Iterationen\n", i); 

    free(tmp);
   
    //Verify if the maximum number of iterations was not exceeded
    return (i >= max_iter) ? 1 : 0;
}

int calculate_eigenvalue(unsigned long long dimension, unsigned long long **valid_signatures, long long *counts, int width, double *eig, double *vec){
    return calculate_eigenvalue2(dimension, valid_signatures, counts, width, eig, vec, MAX_ITER, EPSILON);
}

void v_init(unsigned long long dimension, double *v){
    for(unsigned long long i = 0; i < dimension; i++){
		v[i] = (double)rand() / RAND_MAX;
	}
}

void v_copy(unsigned long long dimension, double *vresult , double *v){
    memcpy(vresult, v, sizeof(double) * dimension);
}

double v_abs(unsigned long long dimension, double *v){
    return sqrt(v_v_mul(dimension, v, v));
}

void v_v_sub(unsigned long long dimension, double *v, double *vresult){
	
    for(unsigned long long i = 0; i < dimension; i++){
		vresult[i] -= v[i];
	}
}

void v_normalize(unsigned long long dimension, double *v){
    double length = v_abs(dimension, v);
	
    v_c_mul(dimension, v, 1.0 / length);
}

void v_c_mul(unsigned long long dimension, double *vresult, double factor){
	
    for(unsigned long long i = 0; i < dimension; i++){
		vresult[i] *= factor;
	}
}

double v_v_mul(unsigned long long dimension, double *v1, double *v2){
    double sum = 0;
	
    for(unsigned long long i = 0; i < dimension; i++){
        sum += v1[i] * v2[i];
	}
	
    return sum;
}

double v_mr_mul(unsigned long long *v1, double *v2){

    double sum = 0;
	
    for(int i = 1; i < v1[0]+1; i++){		
        sum += v2[v1[i]];
    }
    return sum;
}

void m_v_mul(unsigned long long dimension, double *vresult, unsigned long long **valid_signatures, long long *counts, double *v, int width)
{
	
	unsigned long long a = 0;
	unsigned long long signature_index = 0;
	
	for(int i = 0; i<GROUP_SIZE; i++){
		for(long long j = 0; j<counts[i]; j++){
			
			unsigned long long *successor_signatures = (unsigned long long*)malloc(4*sizeof(unsigned long long));

			int length = compute_successor_signatures(successor_signatures, valid_signatures[i][j], width);
			
			unsigned long long *row =(unsigned long long*)malloc((length + 1) * sizeof(unsigned long long));
			row[0] = length;
			
			for(int b = 0; b<length; b++){
				
				signature_index = find_index(valid_signatures, successor_signatures[b], counts, width);
				
				row[b+1] = signature_index;
			}
			
			free(successor_signatures);

			vresult[a] = v_mr_mul(row, v);		

			a++;		
			free(row);
		}
	}
}
#include "transfer_matrix_simple4.h"
#include "matrix_simple4.h"

int calculate_eigenvalue2(unsigned long long dimension, int w, unsigned long long **S, double *eig, double *vec,
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
        m_v_mul(dimension, tmp, vec, w, S);

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
	// The iteration will stop when eather the sufficient error margin or the maximum number of iterations has been reached
    while((i < max_iter) && delta > epsilon);
	
    printf("%u Iterationen\n", i); 

    free(tmp);
   
    //Verify if the maximum number of iterations was not exceeded
    return (i >= max_iter) ? 1 : 0;
}

int calculate_eigenvalue(unsigned long long dimension, int w, unsigned long long **S, double *eig, double *vec){
    return calculate_eigenvalue2(dimension, w, S, eig, vec, MAX_ITER, EPSILON);
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

void m_v_mul(unsigned long long dimension, double *vresult, double *v, int w, unsigned long long **S){
	
	unsigned long long signature_index = 0;
	
	for(unsigned long long i = 1; i<=dimension; i++){
		
		unsigned long long *successor_signatures = (unsigned long long*)malloc(4*sizeof(unsigned long long));
		
		unsigned long long signature = unrank(i, S, w);
		
		//printf("%llu: ", signature);

		unsigned int length = compute_successor_signatures(successor_signatures, signature, w);
		
		unsigned long long *row =(unsigned long long*)malloc((length + 1) * sizeof(unsigned long long));
		row[0] = length;
		
		for(int j = 0; j<length; j++){
			
			signature_index = rank(successor_signatures[j], S, w) - 1 ;
			//printf("%llu_%llu ",successor_signatures[j], signature_index);

			row[j+1] = signature_index;
		}
		//printf("length: %d\n", lengths[i-1]);
		
		free(successor_signatures); 
		
		vresult[i-1] = v_mr_mul(row, v);
		
		free(row);
	}	
}
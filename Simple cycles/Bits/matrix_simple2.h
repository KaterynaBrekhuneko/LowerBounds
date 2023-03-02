/* 
 * Error margin.
 */
#define EPSILON 1.0e-7

/*
 * Max number of iterations.
 */
#define MAX_ITER 10000000UL

/*
 * Calculates the largest eigenvalue and its corresponding eigenvector of the matrix
 * using the Power Iteration method.
 */
int calculate_eigenvalue2(unsigned long long dimension, unsigned int **matrix, double *eig, double *vec, unsigned short *lengths,
    unsigned long max_iter, double epsilon);

/*
 * Calculates the largest eigenvalue and its corresponding eigenvector of the matrix
 * using the Power Iteration method.
 */
int calculate_eigenvalue(unsigned long long dimension, unsigned int **matrix, double *eig, double *vec, unsigned short *lengths);

/*
 * Initialises the vector v with random values.
 */
void v_init(unsigned long long dimension, double *v);

/*
 * Copies all elements of v into vresult.
 */
void v_copy(unsigned long long dimension, double *vresult , double *v);

/*
 * Returns the absolute value of vector v.
 */
double v_abs(unsigned long long dimension, double *v);

/*
 * Subtracts vector v from vector vresult.
 */
void v_v_sub(unsigned long long dimension, double *v, double *vresult);

/*
 * Normileses the vector v.
 */
void v_normalize(unsigned long long dimension, double *v);

/*
 * Vector constant multiplication.
 */
void v_c_mul(unsigned long long dimension, double *vresult, double constant);

/*
 * Vector vector scalar multiplication.
 */
double v_v_mul(unsigned long long dimension, double *v1, double *v2);

/* 
 * Vector matrix row scalar multiplication.
 * v1 - matrix row
 * v2 - vector
 */
double v_mr_mul(unsigned int *v1, double *v2, unsigned short length);

/*
 * Matrix vector multiplication.
 */
void m_v_mul(unsigned int dimension, unsigned int **matrix, double *v, double *vresult, unsigned short *lengths);

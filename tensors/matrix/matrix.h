#ifndef MATRIX_H
#define MATRIX_H

#include "../vector/vector.h"

/* collection of vectors */
typedef struct {
    /* matrix dimensions */
    int m, n;
    /* matrix data (array of Vectors) */
    Vector* d;
} Matrix_t;

/* collection of vectors */
typedef Matrix_t* Matrix;

/* create a new matrix of size (m, n) filled with zeros */
Matrix matrix_new_zeros_(int m, int n);

/* create a new matrix of size (m, n) filled with random floats in [0, 1] */
Matrix matrix_new_randfloat_(int m, int n);

/* create a new matrix of size (m, n) filled with random ints in [0, high] */
Matrix matrix_new_randint_(int m, int n, int high);

/* free a matrix allocated with new_..._matrix_ */
void matrix_free(Matrix M);

/* print a matrix one line per row */
void matrix_print(Matrix M);

#endif
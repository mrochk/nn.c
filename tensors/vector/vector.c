#include <stdlib.h>
#include <stdio.h>

#include "vector.h"
#include "../../utils/utils.h"

Vector new_zeros_vector(int n) {
    Vector vector = (Vector)malloc(sizeof(Vector));

    vector->n = n;

    vector->data = malloc(sizeof(float) * n);

    for (int i = 0; i < n; i++) { 
        vector->data[i] = 0.0f; 
    }

    return vector;
}

Vector new_random_float_vector(int n) {
    Vector vector = (Vector)malloc(sizeof(Vector));

    vector->n = n;

    vector->data = malloc(sizeof(float) * n);

    for (int i = 0; i < n; i++) { 
        vector->data[i] = randfloat(); 
    }

    return vector;
}

Vector new_random_int_vector(int n, int high) {
    Vector vector = (Vector)malloc(sizeof(Vector));

    vector->n = n;

    vector->data = malloc(sizeof(float) * n);

    for (int i = 0; i < n; i++) { 
        vector->data[i] = randint(high); 
    }

    return vector;
}

void free_vector(Vector v) {
    free(v->data);
    free(v);
}

void print_vector(Vector v) {
    printf("[");
    for (int i = 0; i < v->n; i++) {
        if (i == v->n-1) { printf("%.2f", v->data[i]); } 
        else { printf("%.2f ", v->data[i]); }
    }
    puts("]");
}
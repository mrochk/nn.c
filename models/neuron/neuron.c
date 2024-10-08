#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "../../tensors/tensors.h"
#include "../../activations/activations.h"
#include "../../utils/utils.h"

#include "neuron.h"

Neuron neuron_new_(uint n_inputs, Activation activation) {
    Neuron neuron = (Neuron) malloc(sizeof(Neuron));

    neuron->w = vector_new_randfloat_(n_inputs);
    neuron->b = randfloat();
    neuron->activation = activation;

    return neuron;
}

void neuron_free(Neuron n) {
    vector_free(n->w);
    free(n);
}

void neurons_free(int n, Neuron* neurons) {
    for (int i = 0; i < n; i++) {
        neuron_free(neurons[n]);
    }
}

float neuron_forward(Neuron n, Vector x) {
    /* z = x.T @ w + b */
    float z = dotprod(x, n->w) + n->b;

    switch (n->activation) {
        case Identity: return z;
        case Sigmoid:  return sigmoid(z);
        case ReLU:     return relu(z);
        case Tanh:     return tanh(z);
        default:       assert(0);
    }
}

Vector neuron_forward_batch(Neuron n, Matrix X, Vector preds) {
    assert(n); assert(X); assert(preds);

    for (int i = 0; i < X->m; i++) {
        Vector x = X->d[i];
        preds->d[i] = neuron_forward(n, x);
    }
    return preds;
}
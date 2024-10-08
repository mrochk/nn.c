#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../../tensors/tensors.h"
#include "../../activations/activations.h"
#include "../../utils/utils.h"

#include "nn.h"

/**** layer *******************************************************************/

/* create a new feedforward nn layer */
Layer layer_new_(uint inputs, uint outputs, Activation f) {
    assert(inputs > 0); assert(outputs > 0);

    Layer layer = (Layer)malloc(sizeof(struct Layer_t));

    layer->activation = f, layer->inputs = inputs, layer->outputs = outputs;
    layer->weights = matrix_new_randfloat_(outputs, inputs);
    layer->biases  = vector_new_randfloat_(outputs);

    return layer;
}

/* free the memory allocated to a layer */
void layer_free(Layer layer) {
    assert(layer);

    matrix_free(layer->weights);
    vector_free(layer->biases);
    free(layer);

    return;
}

/* computing z = f(W@x + b) with f being the chosen activation func */
void layer_forward(Layer layer, Vector x, Vector z) {
    assert(layer); assert(x); assert(z);
    assert(layer->inputs  == x->n);
    assert(layer->outputs == z->n);

    /* z = x@W + b */
    matvecmul(layer->weights, x, z);
    vector_add(z, layer->biases);

    /* apply activation func */
    vector_apply(z, layer->activation);

    return;
}

/* computing f(X@W + b) with f being the chosen activation func */
void layer_forward_batch(Layer layer, Matrix X, Matrix Z) {
    assert(layer); assert(X); assert(Z);
    assert(layer->inputs  == X->n);
    assert(X->m == Z->m); assert(layer->outputs == Z->n);

    /* compute the linear combination Z = X@W + b */
    matmul(X, layer->weights, Z);
    matrix_add_vector(Z, layer->biases);

    /* apply activation function */
    matrix_apply(Z, layer->activation);

    return;
}

/**** nn **********************************************************************/

/* create a new feedforward neural network */
NN nn_new_(uint nlayers, UintPair* arch, Activation f) {
    assert(nlayers > 0);

    NN nn = (NN)malloc(sizeof(struct NN_t));

    nn->nlayers = nlayers, nn->activation = f;
    nn->layers = (Layer*)malloc(sizeof(Layer) * nlayers);

    for (int i = 0; i < nlayers; i++) {
        int inputs = arch[i].a, outputs = arch[i].b;

        /* no activation function for last layer */
        if (i == nlayers - 1) {
            nn->layers[i] = layer_new_(inputs, outputs, Identity);
            continue;
        }

        nn->layers[i] = layer_new_(inputs, outputs, f);
    }

    return nn;
}

/* free the memory allocated to NN */
void nn_free(NN nn) {
    assert(nn);

    for (int i = 0; i < nn->nlayers; i++) { layer_free(nn->layers[i]); }
    free(nn->layers);
    free(nn);

    return;
}

/* compute a prediction for a single sample */
void nn_forward(NN nn, Vector x, Vector o) {
    assert(nn); assert(x); assert(o);
    assert(nn->layers[0]->inputs == x->n);
    assert(nn->layers[nn->nlayers-1]->outputs == o->n);

    Vector temp1 = vector_new_from_(x);

    for (int i = 0; i < nn->nlayers; i++) {
        Layer layer = nn->layers[i];

        Vector temp2 = vector_new_zeros_(layer->weights->m);
        matvecmul(layer->weights, temp1, temp2);

        vector_free(temp1);
        temp1 = temp2;
    }

    vector_copy(o, temp1);
    vector_free(temp1);

    return;
}

/* compute predictions for a batch */
void nn_forward_batch(NN nn, Matrix X, Matrix O) {
    assert(nn); assert(X); assert(O);
    assert(nn->layers[0]->inputs == X->n);
    assert(nn->layers[nn->nlayers-1]->outputs == O->n);
    assert(X->m == O->m);
    
    Matrix temp1 = matrix_new_from_(X), temp2;

    for (int i = 0; i < nn->nlayers; i++) {
        Layer layer = nn->layers[i];

        /* to store the result of the layer */
        temp2 = matrix_new_(temp1->m, layer->outputs); 

        layer_forward_batch(layer, temp1, temp2);

        matrix_free(temp1);

        temp1 = temp2;
    }

    matrix_copy(temp2, O);

    matrix_free(temp2);

    return;
}

void nn_update(NN nn, Matrix* dws, Vector* dbs, float lr) {
    assert(nn); assert(dws); assert(dbs);

    for (int l = 0; l < nn->nlayers; l++) {
        Layer layer = nn->layers[l];

        Matrix weights = layer->weights;
        Vector biases = layer->biases;

        Matrix dw = dws[l];
        Vector db = dbs[l];

        for (int i = 0; i < layer->outputs; i++) {
            biases->d[i] -= lr * db->d[i];

            Vector row = weights->d[i];
            for (int j = 0; j < layer->inputs; j++) {
                row->d[j] -= lr * dw->d[i]->d[j];
            }
        }
    }

    return;
}
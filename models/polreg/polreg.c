#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "../../tensors/tensors.h"
#include "../../utils/utils.h"

#include "polreg.h"

Polreg polreg_new_(Vector powers, float lr) {
    int n_features  = powers->n;
    Polreg polreg   = (Polreg) malloc(sizeof(Polreg));
    polreg->bias    = randfloat();
    polreg->weights = vector_new_randfloat_(n_features);
    polreg->powers  = powers;
    polreg->lr      = lr;

    return polreg;
}

void polreg_free(Polreg polreg) {
    vector_free(polreg->powers);
    vector_free(polreg->weights);
    free(polreg); polreg = NULL;
}

float polreg_predict(Polreg polreg, Vector x) {
    assert(x && polreg);
    assert(polreg->weights->n == x->n && polreg->powers->n == x->n);

    float result = polreg->bias;

    for (int i = 0; i < x->n; i++) {
        float n = (float) polreg->powers->d[i];
        result += polreg->weights->d[i] * powf(x->d[i], n);
    }

    return result;
}

Vector polreg_predict_batch(Polreg polreg, Matrix X, Vector preds) {
    for (int i = 0; i < X->m; i++) {
        preds->d[i] = polreg_predict(polreg, X->d[i]);
    }
    return preds;
}
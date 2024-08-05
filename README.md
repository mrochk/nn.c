# nn.c
Multi Layer Perceptron & Linear Regression implementation from scratch in pure C.

Models implemented:
- (multivariate) Linear regression. [\[linreg\]](models/linreg) [\[example\]](examples/linreg)
- (multivariate) Polynomial regression. [\[polreg\]](models/polreg) [\[example\]](examples/polreg)
- Neural Network at the neuron level. [\[neuron\]](models/neuron) [\[example\]](examples/neurons)
<!-- - Multilayer Perceptron. [\[mlp\]](models/mlp) -->

For now the functions derivatives are derived by hand for each example but I plan on building an autograd engine soon too.

For the optimizer we always use the simple gradient descent algorithm.
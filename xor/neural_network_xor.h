#ifndef NEURAL_NETWORK_XOR_H
#define NEURAL_NETWORK_XOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Sigmoid function and its derivative
double sigmoid(double x);
double sigmoid_derivative(double x);

//Init all weights and biases
double init_weight();

//Shuffle function
void shuffle(int *array, size_t n);

#endif

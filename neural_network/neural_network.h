#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

/*
    GLOBAL VARIABLES:
    - number of inputs
    - number of nodes in the hidden layer
    - number of outputs
    - learning rate
    - resolution of the image 
    - number of training sets
*/

int num_inputs = 100;
int num_hidden = 100;
int num_output = 10;
double learning_rate = 0.1f;
int res = 100;
int num_training_sets = 0;

/*
    Helper functions:
    - Sigmoid function and its derivative
    - Init all weights and biases
    - Shuffle function
    - Save weights and biases
    - Load weights and biases
    - Function that converts images into training data
*/

double sigmoid(double x);
double sigmoid_derivative(double x);
double init_weight_bias();
void shuffle(int *array, size_t n);
void save_weights_and_biases(char path[], double** hidden_layer_weights[num_inputs][num_hidden], 
    double** output_layer_weights[num_hidden][num_output], double* hidden_layer_bias[num_hidden], 
    double* output_layer_bias[num_output]);
void load_weights_and_biases(char path[], double** hidden_layer_weights[num_inputs][num_hidden], 
    double** output_layer_weights[num_hidden][num_output], double* hidden_layer_bias[num_hidden], 
    double* output_layer_bias[num_output]);
void convert_images_to_training_data(char path[], double** training_set_inputs[num_training_sets][num_inputs],
    double** training_set_outputs[num_training_sets][num_output]);

/*
    Neural network functions:
*/

int train_network(
    int epochs, char path[], 
    double* hidden_layer[num_hidden],
    double* output_layer[num_output], 
    double* hidden_layer_bias[num_hidden], 
    double* output_layer_bias[num_output], 
    double** hidden_layer_weights[num_inputs][num_hidden], 
    double** output_layer_weights[num_hidden][num_output]
    );
void compute_hidden_layer(double* hidden_layer[], double* hidden_layer_bias[], 
    double** hidden_layer_weights[num_inputs][num_hidden], double training_input[]);
void compute_output_layer(double* output_layer[], double* output_layer_bias[], 
    double** output_layer_weights[num_hidden][num_output], double* hidden_layer[]);

#endif

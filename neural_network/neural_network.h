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

#define num_inputs 256
#define num_hidden 256
#define num_output 10
double learning_rate = 1.0f;
#define res 16
#define num_training_sets 10

/*
        Training data:
--------------------------------
*/
char training_inputs[num_training_sets][25] = {
    "bin/training_set/0.png",
    "bin/training_set/1.png",
    "bin/training_set/2.png",
    "bin/training_set/3.png",
    "bin/training_set/4.png",
    "bin/training_set/5.png",
    "bin/training_set/6.png",
    "bin/training_set/7.png",
    "bin/training_set/8.png",
    "bin/training_set/9.png"
};
/*
        (Training input)
--------------------------------
*/
double training_outputs[num_training_sets][num_output] = {
    {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}
};
/*
        (Training output)
--------------------------------
*/

/*
    Helper functions:
    - Sigmoid function and its derivative
    - Init all weights and biases
    - Shuffle function
    - Save weights and biases
    - Load weights and biases
    - Function that converts images into training data (TO FIX)(not used)
*/

double sigmoid(double x);
double sigmoid_derivative(double x);
double init_weight_bias();
void shuffle(int *array, size_t n);
void save_weights_and_biases(char path[], double** hidden_layer_weights, 
    double** output_layer_weights, double* hidden_layer_bias, double* output_layer_bias);
void load_weights_and_biases(char path[], double** hidden_layer_weights, 
    double** output_layer_weights, double* hidden_layer_bias, double* output_layer_bias);
//void convert_images_to_training_data(char path[], double* training_set_inputs[num_training_sets][num_inputs],
//    double* training_set_outputs[num_training_sets][num_output]);

/*
    Neural network functions:
*/

void train_network(
    int epochs, 
    double hidden_layer[num_hidden],
    double output_layer[num_output], 
    double* hidden_layer_bias, 
    double* output_layer_bias, 
    double** hidden_layer_weights, 
    double** output_layer_weights
    );
double *compute_hidden_layer(double hidden_layer[num_hidden], double* hidden_layer_bias, 
    double** hidden_layer_weights, double training_input[num_inputs]);
double *compute_output_layer(double output_layer[num_output], double* output_layer_bias, 
    double** output_layer_weights, double* hidden_layer);

#endif

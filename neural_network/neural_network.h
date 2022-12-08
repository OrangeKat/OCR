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
const double learning_rate = 0.1f;
#define res 16
#define num_training_sets 10

/*
        Training data:
--------------------------------
*/
char training_inputs[num_training_sets][20] = {
    "images/0.png",
    "images/1.png",
    "images/2.png",
    "images/3.png",
    "images/4.png",
    "images/5.png",
    "images/6.png",
    "images/7.png",
    "images/8.png",
    "images/9.png"
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
void convert_images_to_training_data(char path[], double* training_set_inputs[num_training_sets][num_inputs],
    double* training_set_outputs[num_training_sets][num_output]);

/*
    Neural network functions:
*/

int train_network(
    int epochs, 
    double* hidden_layer[num_hidden],
    double* output_layer[num_output], 
    double* hidden_layer_bias[num_hidden], 
    double* output_layer_bias[num_output], 
    double** hidden_layer_weights[num_inputs][num_hidden], 
    double** output_layer_weights[num_hidden][num_output]
    );
void compute_hidden_layer(double* hidden_layer[], double* hidden_layer_bias[], 
    double** hidden_layer_weights[num_inputs][num_hidden], double training_input[num_hidden]);
void compute_output_layer(double* output_layer[], double* output_layer_bias[], 
    double** output_layer_weights[num_hidden][num_output], double* hidden_layer[]);

#endif

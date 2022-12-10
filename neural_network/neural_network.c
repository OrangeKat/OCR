#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include "neural_network.h"
#include "binarization.c"

/*
    Helper functions:
    - Sigmoid function and its derivative
    - Init all weights and biases
    - Shuffle function
    - Save weights and biases
    - Load weights and biases 
    - Function that converts images into training data
*/

// Activation function
double sigmoid(double x){
    return 1/(1+exp(-x));
}

double tanh(double x){
    return (exp(x)-exp(-x))/(exp(x)+exp(-x));
}

double leaky_relu(double x){
    if(x>0){
        return x;
    }else{
        return 0.01*x;
    }
}

// Derivative of the activation function
double sigmoid_derivative(double x){
    return x*(1-x);
}

double tanh_derivative(double x){
    return 1-x*x;
}

double leaky_relu_derivative(double x){
    if(x>0){
        return 1;
    }else{
        return 0.01;
    }
}

// Init all weights and biases
double init_weight_bias(){
    return ((double)rand())/((double)RAND_MAX);
}

// Shuffle function
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

// Save and load weights and biases
void save_weights_and_biases(char path[], double** hidden_layer_weights, 
    double** output_layer_weights, double* hidden_layer_bias, double* output_layer_bias){
    
    FILE *fp = fopen(path, "w");
    for (int i = 0; i < num_inputs; i++){
        for (int j = 0; j < num_hidden; j++){
            fprintf(fp, "%lf, ", hidden_layer_weights[i][j]);
        }
    }
    fprintf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        for (int j = 0; j < num_output; j++){
            fprintf(fp, "%lf, ", output_layer_weights[i][j]);
        }
    }
    fprintf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        fprintf(fp, "%lf, ", hidden_layer_bias[i]);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < num_output; i++){
        fprintf(fp, "%lf, ", output_layer_bias[i]);
    }
    fclose(fp);
}

void load_weights_and_biases(char path[], double** hidden_layer_weights, 
    double** output_layer_weights, double* hidden_layer_bias, double* output_layer_bias){
    
    FILE *fp = fopen(path, "r");
    double *p = malloc(sizeof(double));
    for (int i = 0; i < num_inputs; i++){
        for (int j = 0; j < num_hidden; j++){
            fscanf(fp, "%lf, ", p);
            hidden_layer_weights[i][j] = *p;
        }
    }
    fscanf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        for (int j = 0; j < num_output; j++){
            fscanf(fp, "%lf, ", p);
            output_layer_weights[i][j] = *p;
        }
    }
    fscanf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        fscanf(fp, "%lf, ", p);
        hidden_layer_bias[i] = *p;
    }
    fscanf(fp, "\n");
    for (int i = 0; i < num_output; i++){
        fscanf(fp, "%lf, ", p);
        output_layer_bias[i] = *p;
    }
    fscanf(fp, "\n");
    fclose(fp);
    free(p);
}

// Function that converts images into training data
/*
void convert_images_to_training_data(char path[], double* training_set_inputs[num_training_sets][num_inputs],
    double* training_set_outputs[num_training_sets][num_output]){
    FILE *fp;
    printf("%s", path);
    fp = fopen(path, "r");
    for (int i = 0; i < num_training_sets; i++){
        char path[100];
        fscanf(fp, "%99s", path);
        training_set_inputs[i] = convert_to_array(path);
        fscanf(fp, "\n");
        for (int j = 0; j < num_output; j++){
            fscanf(fp, "%lf, ", training_set_outputs[i][j]);
        }
        fscanf(fp, "\n");
    }
}
*/

/*
    Neural network functions:
    - train_neural_network
*/ 

// Train neural network
void train_network(
    int epochs, 
    double hidden_layer[num_hidden],
    double output_layer[num_output], 
    double* hidden_layer_bias, 
    double* output_layer_bias, 
    double** hidden_layer_weights, 
    double** output_layer_weights
    ){
    printf("Setting weights and biases...\n");
    // Deprecated doesnt work (TO FIX)
    //convert_images_to_training_data(path, training_inputs, training_outputs);

    // Init weights and biases
    
    for (int i = 0; i < num_inputs; i++) {
        for (int j = 0; j < num_hidden; j++) {
            hidden_layer_weights[i][j] = init_weight_bias();
        }
    }
    
    for (int i = 0; i < num_hidden; i++) {
        hidden_layer_bias[i] = init_weight_bias();
        for (int j = 0; j < num_output; j++) {
            output_layer_weights[i][j] = init_weight_bias();
        }
    }

    for (int i = 0; i < num_output; i++) {
        output_layer_bias[i] = init_weight_bias();
    }
    
    printf("Training network...\n");
    
    //Iterate through all training sets for a number of epochs
    for (int i = 0; i < epochs; i++){
	
	    printf("Epoch %d\n", i);
	    
        //Rearrange training sets in random order
        int training_set_order[num_training_sets];
        for (int i = 0; i < num_training_sets; i++){
            training_set_order[i] = i;
        }

        shuffle(training_set_order, num_training_sets);

        //Iterate through all training sets
        for(int n = 0; n < num_training_sets; n++){
            int x = training_set_order[n];

            double input[num_inputs]; 
            memcpy(input, convert_to_array(training_inputs[x]), num_inputs * sizeof(double));

            // Forward pass
            
            for (int j=0; j<num_hidden; j++) {
                double activation=hidden_layer_bias[j];
                 for (int k=0; k<num_inputs; k++) {
                    activation+=input[k]*hidden_layer_weights[k][j] * 0.01;
                }
                hidden_layer[j] = sigmoid(activation);
            }
            
            for (int j=0; j<num_output; j++) {
                double activation=output_layer_bias[j];
                for (int k=0; k<num_hidden; k++) {
                    activation+=hidden_layer[k]*output_layer_weights[k][j] * 0.01;
                }
                output_layer[j] = sigmoid(activation);
            }
                    
           // Backprop
            
            double deltaOutput[num_output];
            for (int j=0; j<num_output; j++) {
                double errorOutput = (training_outputs[x][j]-output_layer[j]);
                deltaOutput[j] = errorOutput*sigmoid_derivative(output_layer[j]);
            }
            
            double deltaHidden[num_hidden];
            for (int j=0; j<num_hidden; j++) {
                double errorHidden = 0.0f;
                for(int k=0; k<num_output; k++) {
                    errorHidden+=deltaOutput[k]*output_layer_weights[j][k];
                }
                deltaHidden[j] = errorHidden*sigmoid_derivative(hidden_layer[j]);
            }
            
            for (int j=0; j<num_output; j++) {
                output_layer_bias[j] += deltaOutput[j]*learning_rate;
                for (int k=0; k<num_hidden; k++) {
                    output_layer_weights[k][j]+=hidden_layer[k]*deltaOutput[j]*learning_rate;
                }
            }
            
            for (int j=0; j<num_hidden; j++) {
                hidden_layer_bias[j] += deltaHidden[j]*learning_rate;
                for(int k=0; k<num_inputs; k++) {
                    hidden_layer_weights[k][j]+=input[k]*deltaHidden[j]*learning_rate;
                }
            }
        }
    }
    
    printf("\nTraining is done!\n");
    char path[] = "bin/weights_biases.txt";
    save_weights_and_biases(path, hidden_layer_weights, output_layer_weights, hidden_layer_bias, output_layer_bias);
	printf("Weights and biases saved.\n");
    
}

/*
    MAIN FUNCTION (Steps):
    - Ask to load weights and biases or init new ones
        - If load, load weights and biases
        - If init, init weights and biases and train network
    - Compute Hidden lair 
    - Compute Output lair 
*/

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: Input cell image\n");
        return 1;
    }
    double hidden_layer[num_hidden]; //= malloc(num_hidden * sizeof(double));
    double output_layer[num_output]; //= malloc(num_output * sizeof(double)); 

    double* hidden_layer_bias = malloc(num_hidden * sizeof(double)); 
    double* output_layer_bias = malloc(num_output * sizeof(double)); 

    
    double** hidden_layer_weights = malloc(num_inputs * sizeof(double*));
    for (int i = 0; i < num_inputs; i++){
        hidden_layer_weights[i] = malloc(num_hidden * sizeof(double));
    }
    double** output_layer_weights = malloc(num_hidden * sizeof(double*));
    for (int i = 0; i < num_hidden; i++){
        output_layer_weights[i] = malloc(num_output * sizeof(double));
    }

    
    // Ask to load weights and biases or init new ones
    char *answer = malloc(sizeof(char));
    printf("Load weights and biases? (y/n): \n");
    scanf("%c", answer);
    
    if (*answer == 'y'){
        // Load weights and biases
        char path[] = "bin/weights_biases.txt";
        load_weights_and_biases(path, hidden_layer_weights, output_layer_weights, hidden_layer_bias, output_layer_bias);
	    printf("Weights and biases loaded and set.\n");
    } 
    else {
        int epochs = 10000;
        // Init weights and biases and train network
        train_network(epochs, hidden_layer, output_layer, hidden_layer_bias, output_layer_bias, 
            hidden_layer_weights, output_layer_weights);
    }
    
    printf("Treating image: %s...\n", argv[1]);
    
    double input[num_inputs];
    
    memcpy(input, convert_to_array(argv[1]), sizeof(double) * num_inputs);

    // Forward pass
            
    for (int j=0; j<num_hidden; j++) {
        double activation=hidden_layer_bias[j];
         for (int k=0; k<num_inputs; k++) {
            activation+=input[k]*hidden_layer_weights[k][j] * 0.01;
        }
        hidden_layer[j] = sigmoid(activation);
    }
        
    for (int j=0; j<num_output; j++) {
        double activation=output_layer_bias[j];
        for (int k=0; k<num_hidden; k++) {
            activation+=hidden_layer[k]*output_layer_weights[k][j] * 0.01;
        }
        output_layer[j] = sigmoid(activation);
    }
            
    for (int n = 0; n < num_output; n++){
        printf("%f, ", output_layer[n]);
    }

    printf("\n");

    return 1;
    
}

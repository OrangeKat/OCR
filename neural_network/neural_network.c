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

// Sigmoids
double sigmoid(double x){
    return 1/(1+exp(-x));
}

// Sigmoid derivative
double sigmoid_derivative(double x){
    return x*(1-x);
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
    - compute hidden layer
    - compute output layer
*/ 

// Compute hidden layer
double *compute_hidden_layer(double* hidden_layer, double* hidden_layer_bias, 
    double** hidden_layer_weights, double training_input[num_inputs]){
    
    for (int j = 0; j < num_hidden; j++){
        double activation = hidden_layer_bias[j];
        for (int p = 0; p < num_inputs; p++){
            activation += training_input[p] * hidden_layer_weights[p][j];
            
        }
        hidden_layer[j] = sigmoid(activation);
    }
    return hidden_layer;
}

// Compute output layer
double *compute_output_layer(double* output_layer, double* output_layer_bias, 
    double** output_layer_weights, double* hidden_layer){

    for (int j = 0; j < num_output; j++){
        double activation = output_layer_bias[j];
        for (int p = 0; p < num_hidden; p++){
            activation += hidden_layer[p] * output_layer_weights[p][j];
        }
        output_layer[j] = sigmoid(activation);
        printf("%f ", activation);
    }
    printf("\n");
    return output_layer;
}

// Train neural network
void train_network(
    int epochs, 
    double* hidden_layer,
    double* output_layer, 
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
            int k = training_set_order[n];

            double input[num_inputs];
	        memcpy(input, convert_to_array(training_inputs[k]), sizeof(double) * num_inputs);

            //Compute hidden layer
            hidden_layer = compute_hidden_layer(hidden_layer, hidden_layer_bias, hidden_layer_weights, input);

            //Compute output layer
            output_layer = compute_output_layer(output_layer, output_layer_bias, output_layer_weights, hidden_layer);

            //Compute change in output layer
            double delta_output[num_output];
            for (int j = 0; j < num_output; j++){
                double error = training_outputs[n][j] - output_layer[j];
                delta_output[j] = error * sigmoid_derivative(output_layer[j]);
            }

            //Compute change in hidden weights
            double delta_hidden[num_hidden];
            for (int j = 0; j < num_hidden; j++){
                double error = 0.0f;
                for (int p = 0; p < num_output; p++){
                    error += delta_output[p] * output_layer_weights[j][p];
                }
                delta_hidden[j] = error * sigmoid_derivative(hidden_layer[j]);
            }

            //Update output weights
            for (int j = 0; j < num_output; j++){
                output_layer_bias[j] += delta_output[j] * learning_rate;
                for (int p = 0; p < num_hidden; p++){
                    output_layer_weights[p][j] += hidden_layer[p] * delta_output[j] * learning_rate;
                }
            }

            //Update hidden weights
            for (int j = 0; j < num_hidden; j++){
                hidden_layer_bias[j] += delta_hidden[j] * learning_rate;
                for (int p = 0; p < num_inputs; p++){
                    hidden_layer_weights[p][j] += input[p] * delta_hidden[j] * learning_rate;
                }
            }

            printf("Expected: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", 
            training_outputs[k][0], training_outputs[k][1], training_outputs[k][2], 
            training_outputs[k][3], training_outputs[k][4], training_outputs[k][5], 
            training_outputs[k][6], training_outputs[k][7], training_outputs[k][8], 
            training_outputs[k][9]);
            printf("Got: %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", 
            output_layer[0], output_layer[1], output_layer[2], 
            output_layer[3], output_layer[4], output_layer[5], 
            output_layer[6], output_layer[7], output_layer[8], 
            output_layer[9]);
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

int *main(){

    double* hidden_layer = malloc(num_hidden * sizeof(double));
    double* output_layer = malloc(num_output * sizeof(double)); 

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
        int epochs = 1;
        // Init weights and biases and train network
        train_network(epochs, hidden_layer, output_layer, hidden_layer_bias, output_layer_bias, 
            hidden_layer_weights, output_layer_weights);
    }
    
    printf("Treating grid...\n");
    static int grid[81];
    /*
    for (int i = 0; i < 81; i++){
        // Find out the number in the image (input: image of cell, output: number)
        char input[20];
        sprintf(input, "grid/%d.png", i);
        double input_cell[num_inputs];
        memcpy(input_cell, convert_to_array(input), sizeof(double) * num_inputs);
        // Compute Hidden lair
        compute_hidden_layer(hidden_layer, hidden_layer_bias, hidden_layer_weights, input_cell);
        // Compute Output lair
        compute_output_layer(output_layer, output_layer_bias, output_layer_weights, hidden_layer);
        // Find the index of the highest value in the output layer
        int index = 0;
        for (int n = 0; n < num_output; n++){
            if (output_layer[n] > output_layer[index]){
                index = n;
            }
        }
        grid[i] = index;
    }

    return grid;
    */
    
    double input_cell[num_inputs];
    memcpy(input_cell, convert_to_array("bin/training_set/1.png"), sizeof(double) * num_inputs);
    // Compute Hidden lair
    compute_hidden_layer(hidden_layer, hidden_layer_bias, hidden_layer_weights, input_cell);
    // Compute Output lair
    compute_output_layer(output_layer, output_layer_bias, output_layer_weights, hidden_layer);
    // Find the index of the highest value in the output layer
    
    for (int n = 0; n < num_output; n++){
        printf("%f, ", output_layer[n]);
    }
    printf("\n");
    
    return grid;
}

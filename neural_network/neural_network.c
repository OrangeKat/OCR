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
    return (double)rand() / RAND_MAX;
}

// Shuffle function
void shuffle(int *array, size_t n){
    if (n > 1){
        size_t i;
        for (i = 0; i < n - 1; i++){
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

// Save and load weights and biases
void save_weights_and_biases(char path[], double* hidden_layer_weights[num_inputs][num_hidden], 
    double* output_layer_weights[num_hidden][num_output], double* hidden_layer_bias[num_hidden], 
    double* output_layer_bias[num_output]){
    
    FILE *fp;
    fp = fopen(path, "w");
    for (int i = 0; i < num_inputs; i++){
        for (int j = 0; j < num_hidden; j++){
            fprintf(fp, "%lf, ", *hidden_layer_weights[i][j]);
        }
    }
    fprintf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        for (int j = 0; j < num_output; j++){
            fprintf(fp, "%lf, ", *output_layer_weights[i][j]);
        }
    }
    fprintf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        fprintf(fp, "%lf, ", *hidden_layer_bias[i]);
    }
    fprintf(fp, "\n");
    for (int i = 0; i < num_output; i++){
        fprintf(fp, "%lf, ", *output_layer_bias[i]);
    }
    fclose(fp);

}

void load_weights_and_biases(char path[], double* hidden_layer_weights[num_inputs][num_hidden], 
    double* output_layer_weights[num_hidden][num_output], double* hidden_layer_bias[num_hidden], 
    double* output_layer_bias[num_output]){
    
    FILE *fp;
    fp = fopen(path, "r");
    for (int i = 0; i < num_inputs; i++){
        for (int j = 0; j < num_hidden; j++){
            fscanf(fp, "%lf, ", *hidden_layer_weights[i][j]);
        }
    }
    fscanf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        for (int j = 0; j < num_output; j++){
            fscanf(fp, "%lf, ", *output_layer_weights[i][j]);
        }
    }
    fscanf(fp, "\n");
    for (int i = 0; i < num_hidden; i++){
        fscanf(fp, "%lf, ", *hidden_layer_bias[i]);
    }
    fscanf(fp, "\n");
    for (int i = 0; i < num_output; i++){
        fscanf(fp, "%lf, ", *output_layer_bias[i]);
    }
    fscanf(fp, "\n");
    fclose(fp);
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
void compute_hidden_layer(double* hidden_layer[], double* hidden_layer_bias[], 
    double* hidden_layer_weights[num_inputs][num_hidden], double training_input[num_hidden]){
    
    for (int j = 0; j < num_hidden; j++){
        double activation = *hidden_layer_bias[j];
        for (int p = 0; p < num_inputs; p++){
            activation += training_input[p] * *hidden_layer_weights[p][j];
        }
        *hidden_layer[j] = sigmoid(activation);
    }
}

// Compute output layer
void compute_output_layer(double* output_layer[], double* output_layer_bias[], 
    double* output_layer_weights[num_hidden][num_output], double* hidden_layer[]){

    for (int j = 0; j < num_output; j++){
        double activation = *output_layer_bias[j];
        for (int p = 0; p < num_hidden; p++){
            activation += *hidden_layer[p] * *output_layer_weights[p][j];
        }
        *output_layer[j] = sigmoid(activation);
    }
}

// Train neural network
int train_network(
    int epochs, 
    double* hidden_layer[num_hidden],
    double* output_layer[num_output], 
    double* hidden_layer_bias[num_hidden], 
    double* output_layer_bias[num_output], 
    double* hidden_layer_weights[num_inputs][num_hidden], 
    double* output_layer_weights[num_hidden][num_output]
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
    
    
    printf("Training network..\n.");
    //Iterate through all training sets for a number of epochs
    for (int i = 0; i < epochs; i++){
	
	if (i % (epochs / 100) == 0)
	    printf("|");
	    
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
            compute_hidden_layer(hidden_layer, hidden_layer_bias, hidden_layer_weights, input);

            //Compute output layer
            compute_output_layer(output_layer, output_layer_bias, output_layer_weights, hidden_layer);

            //Compute change in output layer
            double delta_output[num_output];
            for (int j = 0; j < num_output; j++){
                double error = input[j] - *output_layer[j];
                delta_output[j] = error * sigmoid_derivative(*output_layer[j]);
            }

            //Compute change in hidden weights
            double delta_hidden[num_hidden];
            for (int j = 0; j < num_hidden; j++){
                double error = 0.0f;
                for (int p = 0; p < num_output; p++){
                    error += delta_output[p] * *output_layer_weights[j][p];
                }
                delta_hidden[j] = error * sigmoid_derivative(*hidden_layer[j]);
            }

            //Update output weights
            for (int j = 0; j < num_output; j++){
                *output_layer_bias[j] += delta_output[j];
                for (int p = 0; p < num_hidden; p++){
                    *output_layer_weights[p][j] += *hidden_layer[p] * delta_output[j] * learning_rate;
                }
            }

            //Update hidden weights
            for (int j = 0; j < num_hidden; j++){
                *hidden_layer_bias[j] += delta_hidden[j];
                for (int p = 0; p < num_inputs; p++){
                    *hidden_layer_weights[p][j] += input[p] * delta_hidden[j] * learning_rate;
                }
            }
        }
    }

    printf("\nTraining is done!\n");
    return 1;
}

/*
    MAIN FUNCTION (Steps):
    - Ask to load weights and biases or init new ones
        - If load, load weights and biases
        - If init, init weights and biases and train network
    - Compute Hidden lair 
    - Compute Output lair 
*/

int main(){

    double* hidden_layer[num_hidden];
    double* output_layer[num_output]; 

    double* hidden_layer_bias[num_hidden]; 
    double* output_layer_bias[num_output]; 

    double* hidden_layer_weights[num_inputs][num_hidden]; 
    double* output_layer_weights[num_hidden][num_output];

    // Ask to load weights and biases or init new ones
    char answer;
    printf("Load weights and biases? (y/n): \n");
    scanf("%c", &answer);

    if (answer == 'y'){
        // Load weights and biases
        char path[] = "bin/weights_biases.txt";
        load_weights_and_biases(path, hidden_layer_weights, output_layer_weights, hidden_layer_bias, output_layer_bias);
	printf("Weights and biases loaded and set.\n");
    } 
    else {
        int epochs = 100000;
        //char path[] = "bin/train.txt";

        // Init weights and biases and train network
        train_network(epochs, hidden_layer, output_layer, hidden_layer_bias, output_layer_bias, 
            hidden_layer_weights, output_layer_weights);

	printf("Done training and weights and biases set.\n");
        printf("Do you want to save weights and biases? (y/n): \n");
        scanf("%c", &answer);

        if (answer == 'y'){
            printf("Enter path where to save weights and biases: \n");
            char path[] = "bin/weights_biases.txt";
            save_weights_and_biases(path, hidden_layer_weights, output_layer_weights, hidden_layer_bias, output_layer_bias);
	    printf("Weights and biases saved.\n");
        }
    }

    /*                  COMMENT OUT WHEN *NOT* TESTING
    -------------------------------------------------------------------------
    */
    printf("Enter path where image is located: \n");
    char input[100]; 
    scanf("%99s", input);
    DIR *dir = opendir(input);
    while (dir == NULL){
        printf("Invalid path. Enter path where image set is located: \n");
        scanf("%99s", input);
        dir = opendir(input);
    }
    /*
    -------------------------------------------------------------------------
    */

    // Find out the number in the image (input: image of cell, output: number)
    double input_cell[num_inputs];
    memcpy(input_cell, convert_to_array(input), sizeof(double) * num_inputs);
    // Compute Hidden lair
    compute_hidden_layer(hidden_layer, hidden_layer_bias, hidden_layer_weights, input_cell);
    // Compute Output lair
    compute_output_layer(output_layer, output_layer_bias, output_layer_weights, hidden_layer);
    // Find the index of the highest value in the output layer
    int index = 0;
    for (int i = 0; i < num_output; i++){
        if (*output_layer[i] > *output_layer[index]){
            index = i;
        }
    }
    return index;
}

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// Función de activación tangente hiperbólica
float tanh_af(float x) {
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

// Cálculo de zj
float feed_forward(float *weights, float *training_data) {
    // Estado interno de la neurona j
    float aj = 0.0;
    for (char i = 0; i < 10; i++) {
        aj += weights[i] * training_data[i];
    }

    // Función de activación sigmoide
    return tanh_af(aj);
}

// Suma de pesos
float sum_k(float *weights, float delta_k) {
    float sum_for = 0.0;
    for (int i = 0; i < 10; i++) {
        sum_for += weights[i] * delta_k;
    }
    return sum_for;
}

// Calculo de delta_j y delta_pesos
void backpropagation(float z_j , float *weightsj, float *weightsk, float *training_data,
                    float expected_output, float learning_rate) {
    
    float delta_k = z_j - expected_output;
    float delta_j = z_j * (1 - z_j) * sum_k(weightsk, delta_k);
    
    // Actualización de pesos
    for (char i = 0; i < 10; i++) {
        weightsj[i] -= learning_rate * delta_j * training_data[i];
        weightsk[i] -= learning_rate * delta_k * z_j;
    }
}

// Cálculo de la salida
float output(float weightsj[10][10], float *weightsk, float *training_data) {
    // Estado interno de las neuronas ocultas
    float est_int_hidden[10] = {0.0};
    // j es movimiento de las entradas
    for (char j = 0; j < 10; j++)
    {
        // i es movimiento de los pesos de una misma entrada para las diferentes neuronas
        for(char i = 0; i < 10; i++) {
            est_int_hidden[i] += weightsj[j][i] * training_data[j];
        }
    }

    // Salida de las neuronas ocultas
    for (char i = 0; i < 10; i++)
    {
        est_int_hidden[i] = tanh_af(est_int_hidden[i]);
    }

    float est_int_out = 0.0;
    // Estado interno de la neurona de salida
    for (char i = 0; i < 10; i++)
    {
        est_int_out += weightsk[i] * est_int_hidden[i];
    }

    // Salida de la neurona de salida
    return tanh_af(est_int_out);
    
}

// Cálculo de error
float error(int expected_output[85], float rn_outputs[85]) {
    float error = 0.0;
    for (char i = 0; i < 85; i++) {
        error += abs((float)expected_output[i] - rn_outputs[i]) < 0.000001 ? 0 : 1;
    }
    return error/85.0;
}

float error2(int expected_output[22], float rn_outputs[22]) {
    float error = 0.0;
    for (char i = 0; i < 22; i++) {
        error += abs((float)expected_output[i] - rn_outputs[i]) < 0.000001 ? 0 : 1;
    }
    return error/22.0;
}

int main() {
    float training_data[85][10];
    int expected_output[85];

    FILE *openedFile = fopen("training_data.csv", "r");
    if(openedFile == NULL){
        printf("Failed to open file\n");
    }

    char line[150];
    for(char i=0; i<85; i++){

        if(fgets(line,sizeof(line),openedFile)==NULL) break;

        char dummy[5];
        if(11!=sscanf(line,
        "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%d%1s",
        &training_data[i][0],
        &training_data[i][1],
        &training_data[i][2],
        &training_data[i][3],
        &training_data[i][4],
        &training_data[i][5],
        &training_data[i][6],
        &training_data[i][7],
        &training_data[i][8],
        &training_data[i][9],
        &expected_output[i],
        &dummy)){
            perror("Incorrect line!\n");
            fprintf(stderr, "Line %d: %s\n", i+1, line);
        }
    }

    fclose(openedFile);

    for(int i=0;i<85;i++){
        if(expected_output[i]==1) expected_output[i] = -1.0;
        else expected_output[i] = 1.0;
    }

    // Salidas de la red
    float rn_outputs[85];

    // Número de neuronas capa ocultas
    char hidden_neurons = 10;
    // Número de neuronas capa de entrada
    char input_neurons = 10;

    // Inicialización de pesos
    float weights_in_hidden[input_neurons][hidden_neurons];
    float weights_hidden_out[hidden_neurons];

    for(char i = 0; i < input_neurons; i++) {
        for(char j = 0; j < hidden_neurons; j++) {
            weights_in_hidden[i][j] = (-1*(rand()%2)) * (float)(rand()) / (float)(rand());
        }
        weights_hidden_out[i] = (-1*(rand()%2)) * (float)(rand()) / (float)(rand());
    }

    // Entrenamiento
    while (1)
    {
        for(char i = 0; i < 85; i++) {
            for(char j = 0; j < 10; j++) {

                float z_j = feed_forward(weights_in_hidden[j], training_data[i]);

                backpropagation(z_j, weights_in_hidden[j], weights_hidden_out, 
                training_data[i], expected_output[i], 0.000005);

            }

            rn_outputs[i] = output(weights_in_hidden, weights_hidden_out, training_data[i]);

        }

        float err = error(expected_output, rn_outputs);

        if(err < 0.18){
            printf("Success!\nTraining error: %f\n", err);
            break;
        }
    }

    // Prueba
    float testing_data[22][10];
    int expected_output_testing[22];

    FILE *opened_file = fopen("testing_data.csv", "r");
    if(opened_file == NULL){
        printf("Failed to open file\n");
    }

    char line2[150];
    for(char i=0; i<85; i++){

        if(fgets(line,sizeof(line),opened_file)==NULL) break;

        char dummy[5];
        if(11!=sscanf(line,
        "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%d%1s",
        &testing_data[i][0],
        &testing_data[i][1],
        &testing_data[i][2],
        &testing_data[i][3],
        &testing_data[i][4],
        &testing_data[i][5],
        &testing_data[i][6],
        &testing_data[i][7],
        &testing_data[i][8],
        &testing_data[i][9],
        &expected_output_testing[i],
        &dummy)){
            perror("Incorrect line!\n");
            fprintf(stderr, "Line %d: %s\n", i+1, line);
        }
    }

    fclose(opened_file);

    for(int i=0;i<22;i++){
        if(expected_output_testing[i]==1) expected_output_testing[i] = -1.0;
        else expected_output_testing[i] = 1.0;
    }

    float rn_outputs_testing[22];

    for (char i = 0; i < 22; i++)
    {
        rn_outputs_testing[i] = output(weights_in_hidden, weights_hidden_out, testing_data[i]);
    }

    printf("Testing error: %f\n", error2(expected_output_testing, rn_outputs_testing));
    

    return 0;
}
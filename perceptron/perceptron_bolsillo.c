#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int g(float w[10], float xi[10]);

// Función de activación tangente hiperbólica
float tanh_af(float x) {
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

int main(){
    float trainingData[85][10];
    int expectedClasses[85];

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
        &trainingData[i][0],
        &trainingData[i][1],
        &trainingData[i][2],
        &trainingData[i][3],
        &trainingData[i][4],
        &trainingData[i][5],
        &trainingData[i][6],
        &trainingData[i][7],
        &trainingData[i][8],
        &trainingData[i][9],
        &expectedClasses[i],
        &dummy)){
            perror("Incorrect line!\n");
            fprintf(stderr, "Line %d: %s\n", i+1, line);
        }
    }

    fclose(openedFile);

    for(int i=0;i<85;i++){
        if(expectedClasses[i]==1) expectedClasses[i] = -1;
        else expectedClasses[i] = 1;
    }

    FILE *fileToWrite = fopen("weights_perceptron.csv", "a+");

    // Inicializacion de pesos
    float w[10];

    for(char i = 0; i < 10; i++) {
        w[i] = (-1*(rand()%2)) * (float)(rand()) / (float)(rand());
    }

    float wBolsillo[10];
    char i = 0;
    char V = 0, Vmax = 0;
    while(1)
    {
        if(g(w, trainingData[i])*expectedClasses[i] > 0){
            V++;
            if(V>Vmax){
                Vmax = V;
                for(char j=0;j<10;j++){
                    wBolsillo[j] = w[j];
                }
            }
        } else {
            for(char j=0;j<10;j++){
                w[j] = w[j] + 1*expectedClasses[i]*trainingData[i][j];
            }
            V = 0;
        }            

        /*for(char j=0;j<10;j++){
            fprintf(fileToWrite, "%f,", wBolsillo[j]);
        }
        fprintf(fileToWrite, "\n");*/
        printf("Vmax: %d\n", Vmax);
        if(Vmax==10) break;

    }

    for(char j=0;j<10;j++){
        fprintf(fileToWrite, "%f,", wBolsillo[j]);
    }
    fprintf(fileToWrite, "\n----------------\n");

    fclose(fileToWrite);

    return 0;
}

int g(float w[10], float xi[10]){
    float value = 0;
    // calculo de estado interno
    for(int j=0;j<10;j++){
        value += w[j]*xi[j];
    }
    // función de activación
    return tanh_af(value);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float e(float g[22], int y[22]);

// Función de activación tangente hiperbólica
float tanh_af(float x) {
    return (exp(x) - exp(-x)) / (exp(x) + exp(-x));
}

int main(){

    float testingData[22][10];
    int expectedOutput[22];

    FILE *openedFile = fopen("testing_data.csv", "r");
    if(openedFile == NULL){
        printf("Failed to open file\n");
        exit(-1);
    }

    char line[150];
    for(char i=0; i<22; i++){

        if(fgets(line,sizeof(line),openedFile)==NULL) break;

        char dummy[5];
        if(11!=sscanf(line,
        "%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%d%1s",
        &testingData[i][0],
        &testingData[i][1],
        &testingData[i][2],
        &testingData[i][3],
        &testingData[i][4],
        &testingData[i][5],
        &testingData[i][6],
        &testingData[i][7],
        &testingData[i][8],
        &testingData[i][9],
        &expectedOutput[i],
        &dummy)){
            perror("Incorrect line!\n");
            fprintf(stderr, "Line %d: %s\n", i+1, line);
        }
    }

    fclose(openedFile);

    for(int i=0;i<22;i++){
        expectedOutput[i] = (expectedOutput[i]==1) ? -1 : 1;
    }

    FILE *fileToWrite = fopen("testing_perceptron.csv", "a+");

    float w[10] = {-3.488275,-0.419555,-2.016530,-0.963126,-4.025929,-0.690719,-2.062221,-0.129412,-0.129412,-3.000000};
    float yOut[22];
    for(int j=0; j<22; j++){
        yOut[j] = 0;
        for(int k=0; k<10; k++){
            yOut[j] += w[k]*testingData[j][k];
        }
        yOut[j] = tanh_af(yOut[j]);
    }
    fprintf(fileToWrite, "If Out data is 1, then the output is correct\n");
    fprintf(fileToWrite, "Out data: ");
    for(char i=0; i<22; i++){
        fprintf(fileToWrite, "%f ", yOut[i]*expectedOutput[i]);
    }
    fprintf(fileToWrite, "\nWeights: ");
    for(char i=0; i<10; i++){
        fprintf(fileToWrite, "%f,", w[i]);
    }

    fprintf(fileToWrite, "\nAverage error for testing: %f", e(yOut,expectedOutput));
    fprintf(fileToWrite,"\n-------------------------------------------------------------------------------------------------\n");

    fclose(fileToWrite);
    return 0;
}

float e(float g[22], int y[22]){
    float er = 0;
    for(int j=0; j<22; j++)
        er += (g[j]*y[j]>0) ? 0 : 1;
    return (er/22);
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    float testingData[22][10];
    int expectedOutput[22];

    float e(float g[22], int y[22]);

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

    FILE *fileToWrite = fopen("testing_lms.csv", "a+");

    float w[10] = {-0.586564,0.748177,0.030264,0.414482,-0.192919,-0.074082,1.600253,0.285174,0.285174,0.090902};
    float yOut[22];
    for(int j=0; j<22; j++){
        yOut[j] = 0;
        for(int k=0; k<10; k++){
            yOut[j] += w[k]*testingData[j][k];
        }
        if(yOut[j]<1.5) yOut[j] = 1;
        else yOut[j] = 2;
    }
    fprintf(fileToWrite, "Out data: ");
    for(char i=0; i<22; i++){
        fprintf(fileToWrite, "%f ", yOut[i]);
    }
    fprintf(fileToWrite, "\nWeights: ");
    for(char i=0; i<10; i++){
        fprintf(fileToWrite, "%f,", w[i]);
    }

    fprintf(fileToWrite, "\nAverage error for testing: %f\n", e(yOut,expectedOutput));
    fprintf(fileToWrite,"-------------------------------------------------------------------------------------------------\n");

    fclose(fileToWrite);
    return 0;
}

float e(float g[22], int y[22]){
    float er = 0;
    for(int j=0; j<22; j++)
        er += abs(g[j]-y[j]);
    return (er/22);
}
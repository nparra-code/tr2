#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void g(float w[10], float xi[85][10], float gValues[85]);
float e_allData(float g[85], int y[85]);
void wt1(float w[10], float mu, float er, float xi[10]);


int main()
{
    float trainingData[85][10];
    int expectedClasses[85];

    FILE *openedFile = fopen("training_data.csv", "r");
    if(openedFile == NULL){
        printf("Failed to open file\n");
        exit(-1);
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

    FILE *fileToWrite = fopen("weights_lms.csv", "a+");

    // Random weights
    float w[10];
    for(char i=0;i<sizeof(w);i++){
        w[i] = rand() % 4;
    }

    float mu = 0.2, error_lim = 0.03;
    float error_allData;
    float internalStates[85];
    char i = 0;
    fprintf(fileToWrite, "Iteraciones con paso: %f y error máximo: %f\n", mu, error_lim);
    while (1)
    {
        
        g(w,trainingData,internalStates);
        float error = internalStates[i]-expectedClasses[i];
        error_allData = e_allData(internalStates,expectedClasses);

        if(error_allData < error_lim) break;
        wt1(w,mu,error,trainingData[i]);
        if(i==84) i = 0;
        else i++;
    }

    fprintf(fileToWrite, "Pesos: ");
    for(int a=0; a<10; a++){
        fprintf(fileToWrite, "%f,", w[a]);
    }

    fprintf(fileToWrite, "\nAvg. Error: %f\n\n", error_allData);

    fprintf(fileToWrite,"-------------------------------------------------------------------------------------------------\n");
    fclose(fileToWrite);

    return 0;
}

void g(float w[10], float xi[85][10], float gValues[85]){
    for(int j=0; j<85; j++){
        gValues[j] = 0;
        for(int k=0; k<10; k++){
            gValues[j] += w[k]*xi[j][k];
        }
        
    }
}

float e_allData(float g[85], int y[85]){
    float er = 0;
    float aux;
    for(int j=0; j<85; j++){
        if(g[j]<1.5) aux = 1;
        else if (g[j]>=1.5) aux = 2;
        
        er += abs(g[j]-y[j]);
    }
    return (er/85);
}

void wt1(float w[10], float mu, float er, float xi[10]){
    for(char j = 0; j<10; j++){
       w[j] -= mu*er*xi[j];
    }
}
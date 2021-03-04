#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeMatrix(float **matrix){
    free(*matrix);
    //free(*matrix);
}

void initMatrix(float **matrix, int N, int M){
    matrix=malloc(M * sizeof(*matrix));
    float *data = malloc(M * N * sizeof(*data));
    for(int i=0; i<M; i++, data+=N){
        matrix[i]=data;
    }
    
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            matrix[i][j]=0;
        }
    }
}

int readValue(float *matrix, int i, int j){
    return matrix[i];
}

void changeValue(float **matrix, int i, int j, float new){
    float arr[i][j];
    arr[i][j]=new;
}

void writeFile(FILE *fp, int hr, float **matrix, int M, int N){
    if(hr=1){//if the file is human readable, then
        for(int i=0; i<M; i++){
            for(int j=0; j<N; j++){
                fprintf(fp, "%f", matrix[i][j]);
            }
        }
    }
    else{
        
    }
}

void readFile(FILE *fp, int hr, int M, int N){
    if(hr=1){//if the file is human readable, then

    }
    else{
        
    }
}
void printMatrix(float **matrix, int M, int N){
    for(int j = 0; j < M; j++ ){
        for(int i = 0; i < N; i ++){
            printf( "%f ", matrix[j][i] );
        }
        printf( "\n" );
    }
}

int main(){
    int N=5, M=5;
    float **matrix;
    initMatrix(matrix, N, M);
    //printMatrix(matrix, M, N);
    
    freeMatrix(matrix);
}

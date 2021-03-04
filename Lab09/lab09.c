/***
 * Matrix structure and accompanying functions.
 ****/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "matrix.h"


void test_matrix_matrix_add_threaded(){
    size_t NT;
    size_t ncols;
    size_t nrows;
    int theNum;
    
    printf("please input number of threads:");
    scanf("%zu", &NT);
    printf("please input number of rows:");
    scanf("%zu", &nrows);
    while(nrows<NT){
        printf("invalid input. Please input number of rows >= threads:");
        scanf("%zu", &nrows);
    }
    printf("please input number of cols:");
    scanf("%zu", &ncols);
    
    int N=nrows*ncols;
    pthread_t threads[NT];
    tinfo tdata[NT];
    matrix * a = matrix_rand(nrows, ncols, 30);
    matrix * b = matrix_rand(nrows, ncols, 30);
    matrix * c = matrix_zero(nrows, ncols);
    
        printf("Matrix A:\n");
        matrix_print(a);
        printf("Matrix B:\n");
        matrix_print(b);
        printf("Matrix C Before:\n");
        matrix_print(c);
    
    
    clock_t before = clock();

    for(size_t i=0; i < NT; ++i){
        tdata[i].NT=NT;
        tdata[i].tid = i;
        tdata[i].matA = a;
        tdata[i].matB = b;
        tdata[i].matC = c;
        
        if(pthread_create(&threads[i], NULL, matrix_matrix_add_threaded, (void *)(tdata + i)) != 0){
            printf("Could not create thread id %zu!\n", i);
            return;
        }
    }
    for(size_t i=0; i < NT; ++i){
        if(pthread_join(threads[i], NULL) != 0){
            printf("Could not join thread id %zu!\n", i);
            return;
        }
    }
    clock_t difference = clock()-before;
    printf("Matrix C After:\n");

    matrix_print(c);
    matrix_free(&a);
    matrix_free(&b);
    matrix_free(&c);
    printf("\n executed in %lu mil \n", difference);
}






void test_matrix_int_add_threaded(){
    size_t NT;
    size_t ncols;
    size_t nrows;
    int theNum;
    
    printf("please input number of threads:");
    scanf("%zu", &NT);
    printf("please input number of rows:");
    scanf("%zu", &nrows);
    while(nrows<NT){
        printf("invalid input. Please input number of rows >= threads:");
        scanf("%zu", &nrows);
    }
    printf("please input number of cols:");
    scanf("%zu", &ncols);
    printf("please input the Number:");
    scanf("%d", &theNum);

    int N=nrows*ncols;
    pthread_t threads[NT];
    tinfo tdata[NT];
    matrix * a = matrix_rand(nrows, ncols, 30);
    printf("Before:\n");
    matrix_print(a);
    clock_t before = clock();
    for(size_t i=0; i < NT; ++i){
        tdata[i].NT=NT;
        tdata[i].tid = i;
        tdata[i].matA = a;
        tdata[i].num = theNum;
        //tdata[i].data = data;
        if(pthread_create(&threads[i], NULL, matrix_int_add_inplace_threaded, (void *)(tdata + i)) != 0){
            printf("Could not create thread id %zu!\n", i);
            return;
        }
    }
    for(size_t i=0; i < NT; ++i){
        if(pthread_join(threads[i], NULL) != 0){
            printf("Could not join thread id %zu!\n", i);
            return;
        }
    }
    clock_t difference = clock()-before;
    printf("After:\n");
    matrix_print(a);
    matrix_free(&a);
    printf("\n executed in %lu mil \n", difference);
}

char test_matrix_int_add(){
    int nrows = 4;
    int ncols = 7;
    matrix *a = matrix_one(nrows, ncols);
    if(!a){
        return 0;
    }
    matrix_int_add_inplace(a, 1);
    
    for(int i=0; i < nrows; ++i){
        for(int j=0; j < ncols; ++j){
            if(a->rows[i][j] != 2){
                matrix_free(&a);
                return 0;
            }
        }
    }
    
    matrix_free(&a);
    
    return 1;
}

char test_matrix_matrix_multiply(){
    int nrows = 2;
    int ncols = 3;
    int ncols2 = 4;
    matrix *a, *b, *c;
    
    int adata[] = {2, 1, 4, 0, 1, 1};
    int bdata[] = {6, 3, -1, 0, 1, 1, 0, 4, -2, 5, 0, 2};
    int cdata[] = {5, 27, -2, 12, -1, 6, 0, 6};
    
    // create matrices
    a = matrix_create(nrows, ncols);
    b = matrix_create(ncols, ncols2);
    c = matrix_create(nrows, ncols2);
    
    if(!a || !b || !c){
        return 0;
    }
    
    // copy data for a and b
    memcpy(a->data, adata, nrows * ncols * sizeof(int));
    memcpy(b->data, bdata, ncols * ncols2 * sizeof(int));
    
    // execute mat-mat-multiply
    if(!matrix_matrix_multiply(a, b, c)){
        matrix_free(&a);
        matrix_free(&b);
        matrix_free(&c);
        return 0;
    }
    
    // verify result is correct
    for(int i=0; i < nrows; ++i){
        for(int j=0; j < ncols2; ++j){
            if(c->data[i*ncols2 + j] != cdata[i*ncols2 + j]){
                matrix_free(&a);
                matrix_free(&b);
                matrix_free(&c);
                return 0;
            }
        }
    }
    matrix_free(&a);
    matrix_free(&b);
    matrix_free(&c);
    return 1;
}


int main()
{
    char input;
    printf("Please input command\n");
    scanf("%c", &input);
    if(input=='a')
        test_matrix_int_add_threaded();
    if(input=='m')
        test_matrix_matrix_add_threaded();

    //printf("test_matrix_int_add: %s\n", test_matrix_int_add() ? "pass" : "fail");
    //printf("test_matrix_matrix_multiply: %s\n", test_matrix_matrix_multiply() ? "pass" : "fail");
}

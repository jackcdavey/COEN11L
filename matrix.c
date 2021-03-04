/***
 * Matrix structure and accompanying functions.
 ****/
#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>


/**
 * Create an empty matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @return       Pointer to structure or NULL
 */
matrix * matrix_create(const int nrows, const int ncols)
{
    if(nrows < 1 || ncols < 1){
        return NULL;
    }
    matrix * mat = (matrix *) malloc(sizeof(matrix));
    if(!mat){
        return NULL;
    }
    mat->nrows = nrows;
    mat->ncols = ncols;
    mat->data = (int *) malloc(sizeof(int) * nrows * ncols);
    if(!mat->data){
        // free structure memory since we cannot allocate space for data
        free(mat);
        return NULL;
    }
    mat->rows = (int **) malloc(sizeof(int *) * nrows);
    if(!mat->rows){
        // free structure memory since we cannot allocate space for rows
        free(mat->data);
        free(mat);
        return NULL;
    }
    for(int i=0; i < nrows; ++i){
        mat->rows[i] = mat->data + i * ncols;
    }
    return mat;
}

/**
 * Initialize all elements of the matrix with a given value.
 * @param  mat   Matrix to be initialized
 * @param val Value to initialize elements with
 */
void matrix_init(matrix * mat, const int val){
    for(long i=0; i < (long) mat->nrows * mat->ncols; ++i){
        mat->data[i] = val;
    }
}

/**
 * Create a zero matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @return       Pointer to structure
 */
matrix * matrix_zero(const int nrows, const int ncols){
    matrix * mat = matrix_create(nrows, ncols);
    if(mat){
        memset(mat->data, 0, sizeof(int) * nrows * ncols);
    }
    return mat;
}

/**
 * Create a one matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @return       Pointer to structure
 */
matrix * matrix_one(const int nrows, const int ncols){
    matrix * mat = matrix_create(nrows, ncols);
    if(mat){
        matrix_init(mat, 1);
    }
    return mat;
}

/**
 * Create a random matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @param  mxval Maximum random value
 * @return       Pointer to structure
 */
matrix * matrix_rand(const int nrows, const int ncols, const int mxval)
{
    matrix * mat = matrix_create(nrows, ncols);
    if(mat){
        for(long i=0; i < (long) mat->nrows * mat->ncols; ++i){
            mat->data[i] = rand() % mxval;
        }
    }
    return mat;
}

/**
 * Make a copy of a matrix
 * @param  mat Matrix to be copied
 * @return     Pointer to new matrix
 */
matrix * matrix_copy(const matrix * mat){
    matrix * cpy = matrix_create(mat->nrows, mat->ncols);
    if(cpy){
        memcpy(cpy->data, mat->data, sizeof(int) * mat->nrows * mat->ncols);
    }
    return cpy;
}

/**
 * Free matrix data structure
 * @param matp Reference to matrix
 */
void matrix_free(matrix ** matp){
    free((*matp)->rows);
    free((*matp)->data);
    free(*matp);
    *matp = NULL;
}

/**
 * Read value at given location in the matrix
 * @param  mat Matrix to be changed
 * @param  rid Row ID
 * @param  cid Column ID
 * @return     Value at that location, INT_MIN if invalid
 */
int matrix_get(matrix * mat, const int rid, const int cid){
    if( !mat || rid < 0 || rid >= mat->nrows || cid < 0 || cid >= mat->ncols ){
        return INT_MIN;
    }
    return mat->data[rid * mat->ncols + cid];
}

/**
 * Set value at given location in matrix
 * @param  mat Matrix to be changed
 * @param  rid Row ID
 * @param  cid Column ID
 * @param  val Value to be set
 * @return     1 if success, 0 otherwise.
 */
char matrix_set(matrix * mat, const int rid, const int cid, const int val){
    if( !mat || rid < 0 || rid >= mat->nrows || cid < 0 || cid >= mat->ncols ){
        return 0;
    }
    mat->data[rid * mat->ncols + cid] = val;
    return 1;
}

/**
 * Read a matrix from a file
 * @param  path Path for matrix file
 * @param  type Type of file ('t' for text/human-readable, 'b' for binary)
 * @return      Pointer to matrix, NULL otherwise
 */
matrix * matrix_read(const char * path, const char type)
{
    int nrows, ncols, v;
    size_t ret;
    matrix * mat = NULL;
    // open file
    FILE * fin  = fopen (path, type == 'b' ? "rb" : "r");
    if(!fin){
        return NULL;
    }
    if (type == 'b'){
        // read matrix size
        ret  = fread(&nrows, sizeof(int), 1, fin);
        if(ret != 1){
            fclose(fin);
            return NULL;
        }
        ret  = fread(&ncols, sizeof(int), 1, fin);
        if(ret != 1){
            fclose(fin);
            return NULL;
        }
        // create matrix structure
        mat = matrix_create(nrows, ncols);
        if(!mat){
            fclose(fin);
            return NULL;
        }
        // read in the values of the matrix
        ret = fread(&mat->data, sizeof(int), (size_t) nrows * (size_t) ncols, fin);
        if(ret != (size_t) nrows * (size_t) ncols){
            fclose(fin);
            matrix_free(&mat);
            return NULL;
        }
    } else if(type == 't'){
        // the first row contains nrows and ncols
        ret = fscanf(fin, "%d %d\n", &nrows, &ncols);
        if(ret != 2){
            fclose(fin);
            return NULL;
        }
        // create matrix structure
        mat = matrix_create(nrows, ncols);
        if(!mat){
            fclose(fin);
            return NULL;
        }
        for(int i=0; i < nrows; ++i){
            for(int j=0; j < ncols; ++j){
                ret = fscanf(fin, " %d", &v); // space means it should ignore whitespace before the number
                if(ret != 1){
                    fclose(fin);
                    matrix_free(&mat);
                    return NULL;
                }
                mat->data[i * ncols + j] = v;
            }
        }
    } else { // invalid type requested
        fclose(fin);
        return NULL;
    }
    return mat;
}

/**
 * Write a matrix to a file
 * @param  mat  Matrix to be written
 * @param  path Path for matrix file
 * @param  type Type of file ('t' for text/human-readable, 'b' for binary)
 * @return      1 if success, 0 otherwise
 */
char matrix_write(const matrix * mat, const char * path, const char type)
{
    FILE * fout = fopen (path, type == 'b' ? "wb" : "w");
    if(!fout){
        return 0;
    }
    if(type == 't'){
        // write matrix size (so we can easily read without scanning)
        if(fprintf(fout, "%d %d\n", mat->nrows, mat->ncols) < 0){
            fclose(fout);
            return 0;
        }
        // write the values
        for(int i=0; i < mat->nrows; ++i){
            for(int j=0; j < mat->ncols; ++j){
                if(fprintf(fout, "%d ", mat->data[i * mat->ncols + j]) < 0){
                    fclose(fout);
                    return 0;
                }
            }
            // write newline at end of each line
            if(fprintf(fout, "\n") < 0){
                fclose(fout);
                return 0;
            }
        }
    } else if(type == 'b'){
        // write matrix size
        if(fwrite(&(mat->nrows), sizeof(int), 1, fout) != 1){
            fclose(fout);
            return 0;
        }
        if(fwrite(&(mat->ncols), sizeof(int), 1, fout) != 1){
            fclose(fout);
            return 0;
        }
        // write data
        if(fwrite(&(mat->data), sizeof(int), mat->nrows * mat->ncols, fout) != mat->nrows * mat->ncols){
            fclose(fout);
            return 0;
        }
    } else {
        fclose(fout);
        return 0;
    }
    // done
    return 1;
}

// Operations

/**
 * Multiply a matrix by a constant in place
 * @param mat Matrix
 * @param val Constant to multiply by
 */
void matrix_int_multiply_inplace(matrix * mat, const int val)
{
    for(int i=0; i < mat->nrows; ++i){
        for(int j=0; j < mat->ncols; ++j){
            mat->data[i * mat->ncols + j] *= val;
        }
    }
}


/**
 * Divide a matrix by a constant in place
 * @param mat Matrix
 * @param val Constant to divide by
 */
void matrix_int_divide_inplace(matrix * mat, const int val)
{
    for(int i=0; i < mat->nrows; ++i){
        for(int j=0; j < mat->ncols; ++j){
            mat->data[i * mat->ncols + j] /= val;
        }
    }
}

/**
 * Add a constant to a matrix in place
 * @param mat Matrix
 * @param val Constant to add
 */
void matrix_int_add_inplace(matrix * mat, const int val)
{
    for(int i=0; i < mat->nrows; ++i){
        for(int j=0; j < mat->ncols; ++j){
            mat->data[i * mat->ncols + j] += val;
        }
    }
}





void * matrix_matrix_add_threaded(void * arg){
    tinfo * info = arg;
    matrix * a=info->matA;
    matrix * b=info->matB;
    matrix * c=info->matC;
    
    if (a->nrows != b->nrows || b->nrows != c->nrows || a->ncols != b->ncols || b->ncols != c->ncols){
        return 0; // invalid matrix sizes
    }
    
    size_t N=a->ncols*a->nrows;
    size_t bucket_size = ceil(N, info->NT);
    

    //printf("Thread %zu\n", info->tid);
    for(int j=info->tid*bucket_size; j < info->tid*bucket_size+bucket_size; ++j){
        size_t calc = info->tid*bucket_size+j;
        if (calc > N) break;
        //printf("t%zu ; ", info->tid);
        info->matC->data[calc] += info->matA->data[calc];
    }
    for(int j=0; j < bucket_size; ++j){
        size_t calc = info->tid*bucket_size+j;
        if (calc > N) break;
        //printf("t%zu ; ", info->tid);
        info->matC->data[calc] += info->matB->data[calc];
    }
    //printf("\n");
    return NULL;
}







void * matrix_int_add_inplace_threaded(void * arg){
    tinfo * info = arg;
    matrix * a=info->matA;
    size_t N=a->ncols*a->nrows;
    //printf("%d", N);
    size_t bucket_size = ceil(N, info->NT);

    for(int j=info->tid*bucket_size; j < info->tid*bucket_size+bucket_size; ++j){
        size_t calc = info->tid*bucket_size+j;
        if (calc > N) break;
        //printf("t%zu ; ", info->tid);
        info->matA->data[calc] += info->num;
    }
    //printf("\n");
    return NULL;
}

/**
 * Subtract a constant from a matrix in place
 * @param mat Matrix
 * @param val Constant to subtract
 */
void matrix_int_subtract_inplace(matrix * mat, const int val)
{
    for(int i=0; i < mat->nrows; ++i){
        for(int j=0; j < mat->ncols; ++j){
            mat->data[i * mat->ncols + j] -= val;
        }
    }
}

/**
 * Add two matrices and store the result in the third matrix
 * @param a Matrix to add to
 * @param b Matrix to add
 * @param c Matrix to store result in
 */
char matrix_matrix_add(const matrix * const a, const matrix * const b, matrix * const c)
{
    if (a->nrows != b->nrows || b->nrows != c->nrows || a->ncols != b->ncols || b->ncols != c->ncols){
        return 0; // invalid matrix sizes
    }
    int nrows = a->nrows;
    int ncols = a->ncols;
    for(int i=0; i < nrows; ++i){
        for(int j=0; j < ncols; ++j){
            c->data[i * ncols + j] = a->data[i * ncols + j] + b->data[i * ncols + j];
        }
    }
    return 1;
}

/**
 * Subtract one matrix from another and store the result in the third matrix
 * @param a Matrix to subtract from
 * @param b Matrix to subtract
 * @param c Matrix to store result in
 */
char matrix_matrix_subtract(const matrix * const a, const matrix * const b, matrix * const c)
{
    if (a->nrows != b->nrows || b->nrows != c->nrows || a->ncols != b->ncols || b->ncols != c->ncols){
        return 0; // invalid matrix sizes
    }
    int nrows = a->nrows;
    int ncols = a->ncols;
    for(int i=0; i < nrows; ++i){
        for(int j=0; j < ncols; ++j){
            c->data[i * ncols + j] = a->data[i * ncols + j] - b->data[i * ncols + j];
        }
    }
    return 1;
}

/**
 * Multiply two matrices and store the result in the third matrix
 * @param a Matrix to multiply
 * @param b Matrix to multiply by
 * @param c Matrix to store result in
 */
char matrix_matrix_multiply(const matrix * const a, const matrix * const b, matrix * const c)
{
    if (a->ncols != b->nrows || c->nrows != a->nrows || c->ncols != b->ncols){
        return 0; // invalid matrix sizes
    }
    int nrows  = a->nrows;
    int ncols  = a->ncols;
    int ncols2 = b->ncols;
    int v;
    for(int i=0; i < nrows; ++i){
        for(int j=0; j < ncols2; ++j){
            // c_{i,j} = \sum_{k=1}^m a_{i,k} b_{k,j}
            v = 0;
            for(int k=0; k < ncols; ++k){
                v += a->rows[i][k] * b->rows[k][j];
            }
            c->rows[i][j] = v;
        }
    }
    return 1;
}

/**
 * Modify matrix value in both the matrix and its binary back-end
 * @param  ofp Write file pointer for the matrix file
 * @param  mat Matrix structure originally read from ofp
 * @param  rid Row ID
 * @param  cid Column ID
 * @param  val Value to be set
 * @return     1 if success, 0 otherwise.
 */
char matrix_set_write(FILE * ofp, matrix * mat, const int rid, const int cid, const int val)
{
    if( !mat || rid < 0 || rid >= mat->nrows || cid < 0 || cid >= mat->ncols ){
        return 0;
    }
    size_t location = 0;
    // go past nrows and ncols
    location += 2 * sizeof(int);
    // go to the right value in the matrix
    location += (rid * mat->ncols + cid) * sizeof(int);
    // try to go to location in the file
    if(fseek( ofp, location, SEEK_SET) != 0){
        return 0; // could not get to that location in the file
    }
    // write value to file
    if(fwrite(&val, sizeof(int), 1, ofp) != 1){
        return 0; // could not write value to file
    }
    // update data structure value
    mat->data[rid * mat->ncols + cid] = val;
    // done
    return 1;
    
}

void matrix_print(matrix * a){
    printf("\n");
    for(size_t i=0; i < a->nrows; ++i){
        for(size_t j=0; j < a->ncols; ++j){
            printf("%d ", a->rows[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


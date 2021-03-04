/***
 * Matrix structure and accompanying functions.
 ****/
#include <stdio.h>
#include <pthread.h>

#ifndef _MATRIX_H_
#define _MATRIX_H_
#define min(a,b) ((a) < (b) ? (a) : (b))
#define ceil(x,y) (((x) + (y) - 1) / (y))

typedef struct matrix {
    int nrows;
    int ncols;
    int * data;
    int ** rows;
} matrix;

typedef struct tinfo{
    size_t NT;
    int * data;
    matrix * matA;
    matrix * matB;
    matrix * matC;
    int num;
    size_t tid;
}tinfo;


void * matrix_int_add_inplace_threaded(void * arg);
void * matrix_matrix_add_threaded(void * arg);


/**
 * Create an empty matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @return       Pointer to structure
 */
matrix * matrix_create(const int nrows, const int ncols);

/**
 * Initialize all elements of the matrix with a given value.
 * @param  mat   Matrix to be initialized
 * @param val Value to initialize elements with
 */
void matrix_init(matrix * mat, const int val);
/**
 * Create a zero matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @return       Pointer to structure
 */
matrix * matrix_zero(const int nrows, const int ncols);

/**
 * Create a one matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @return       Pointer to structure
 */
matrix * matrix_one(const int nrows, const int ncols);

/**
 * Create a random matrix with a given # of rows and columns.
 * @param  nrows Number of rows
 * @param  ncols Number of columns
 * @param  mxval Maximum random value
 * @return       Pointer to structure
 */
matrix * matrix_rand(const int nrows, const int ncols, const int mxval);

/**
 * Make a copy of a matrix
 * @param  mat Matrix to be copied
 * @return     Pointer to new matrix
 */
matrix * matrix_copy(const matrix * mat);

/**
 * Free matrix data structure
 * @param matp Reference to matrix
 */
void matrix_free(matrix ** matp);

/**
 * Read value at given location in the matrix
 * @param  mat Matrix to be changed
 * @param  rid Row ID
 * @param  cid Column ID
 * @return     Value at that location, INT_MIN if invalid
 */
int matrix_get(matrix * mat, const int rid, const int cid);

/**
 * Set value at given location in matrix
 * @param  mat Matrix to be changed
 * @param  rid Row ID
 * @param  cid Column ID
 * @param  val Value to be set
 * @return     1 if success, 0 otherwise.
 */
char matrix_set(matrix * mat, const int rid, const int cid, const int val);

/**
 * Read a matrix from a file
 * @param  path Path for matrix file
 * @param  type Type of file ('t' for text/human-readable, 'b' for binary)
 * @return      Pointer to matrix, NULL otherwise
 */
matrix * matrix_read(const char * path, const char type);

/**
 * Write a matrix to a file
 * @param  mat  Matrix to be written
 * @param  path Path for matrix file
 * @param  type Type of file ('t' for text/human-readable, 'b' for binary)
 * @return      1 if success, 0 otherwise
 */
char matrix_write(const matrix * mat, const char * path, const char type);

// Operations

/**
 * Multiply a matrix by a constant in place
 * @param mat Matrix
 * @param val Constant to multiply by
 */
void matrix_int_multiply_inplace(matrix * mat, const int val);


/**
 * Divide a matrix by a constant in place
 * @param mat Matrix
 * @param val Constant to divide by
 */
void matrix_int_divide_inplace(matrix * mat, const int val);

/**
 * Add a constant to a matrix in place
 * @param mat Matrix
 * @param val Constant to add
 */
void matrix_int_add_inplace(matrix * mat, const int val);

/**
 * Subtract a constant from a matrix in place
 * @param mat Matrix
 * @param val Constant to subtract
 */
void matrix_int_subtract_inplace(matrix * mat, const int val);

/**
 * Add two matrices and store the result in the third matrix
 * @param a Matrix to add to
 * @param b Matrix to add
 * @param c Matrix to store result in
 */
char matrix_matrix_add(const matrix * const a, const matrix * const b, matrix * const c);

/**
 * Subtract one matrix from another and store the result in the third matrix
 * @param a Matrix to subtract from
 * @param b Matrix to subtract
 * @param c Matrix to store result in
 */
char matrix_matrix_subtract(const matrix * const a, const matrix * const b, matrix * const c);

/**
 * Multiply two matrices and store the result in the third matrix
 * @param a Matrix to multiply
 * @param b Matrix to multiply by
 * @param c Matrix to store result in
 */
char matrix_matrix_multiply(const matrix * const a, const matrix * const b, matrix * const c);

/**
 * Modify matrix value in both the matrix and its binary back-end
 * @param  ofp Write file pointer for the matrix file
 * @param  mat Matrix structure originally read from ofp
 * @param  rid Row ID
 * @param  cid Column ID
 * @param  val Value to be set
 * @return     1 if success, 0 otherwise.
 */
char matrix_set_write(FILE * ofp, matrix * mat, const int rid, const int cid, const int val);

void matrix_print(matrix * a);

#endif /* _MATRIX_H_ */

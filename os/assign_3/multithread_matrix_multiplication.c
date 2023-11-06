#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct matrix {
    int rows, cols;
    int **data;
} MATRIX;

typedef struct thread_data {
    int common, i, j;
    MATRIX mat1, mat2;
} THREAD_DATA;

MATRIX setMatrix();
void displayMatrix(MATRIX);
void getRowColSum(void *);

int main(int argc, char const *argv[]) {
    MATRIX mat1, mat2, mat3;

    mat1 = setMatrix();
    displayMatrix(mat1);
    mat2 = setMatrix();
    displayMatrix(mat2);

    if (mat1.cols != mat2.rows) {
        printf("mulplication isn't possible\n");
        exit(0);
    }

    mat3.rows = mat1.rows;
    mat3.cols = mat2.cols;
    mat3.data = (int **)calloc(mat3.rows, sizeof(int *));

    int common = mat1.cols;
    for (int i = 0; i < mat1.rows; i++) {
        mat3.data[i] = (int *)calloc(mat3.cols, sizeof(int));
        for (int j = 0; j < mat2.cols; j++) {
            int *sum;
            pthread_t *th = (pthread_t *)calloc(1, sizeof(pthread_t));

            THREAD_DATA *para = (THREAD_DATA *)calloc(1, sizeof(THREAD_DATA));
            para->common = common;
            para->i = i;
            para->j = j;
            para->mat1 = mat1;
            para->mat2 = mat2;

            pthread_create(th, NULL, (void *)&getRowColSum, (void *)para);
            pthread_join(*th, (void *)&sum);
            // printf("%d %d %d\n", i, j, *sum);
            mat3.data[i][j] = *sum;
        }
    }
    printf("The multiplied matrix:\n");
    displayMatrix(mat3);

    return 0;
}

MATRIX setMatrix() {
    int rows, cols;
    printf("Enter the number of rows: ");
    scanf("%d", &rows);
    printf("Enter the number of columns: ");
    scanf("%d", &cols);

    int **matrix = (int **)calloc(rows, sizeof(int *));

    printf("Enter the elements rows wise:");
    for (int i = 0; i < rows; i++) {
        int *rowElements = (int *)calloc(cols, sizeof(int));
        for (int j = 0; j < cols; j++) {
            int ele;
            printf("enter elements for index (%d,%d): ", i, j);
            scanf("%d", &ele);
            rowElements[j] = ele;
            // printf("%d \n", rowElements[j]);
        }

        matrix[i] = rowElements;
    }

    MATRIX mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = matrix;
    return mat;
}

void displayMatrix(MATRIX mat) {
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++)
            printf("%d ", mat.data[i][j]);
        printf("\n");
    }
    printf("\n");
}

void getRowColSum(void *para) {
    THREAD_DATA *td = (THREAD_DATA *)para;
    int i = td->i;
    int j = td->j;
    int common = td->common;
    MATRIX mat1 = td->mat1;
    MATRIX mat2 = td->mat2;
    // printf("%d%d", i, j);
    int *sum = (int *)calloc(1, sizeof(int));
    for (int k = 0; k < common; k++)
        *sum += mat1.data[i][k] * mat2.data[k][j];

    pthread_exit((void *)sum);
}
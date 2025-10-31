#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Matrix.h"

struct matrix {
    size_t rows;
    size_t cols;
    float *data;   
};

Matrix mat_create(size_t rows, size_t cols) {
    // allocate the struct
    Matrix m = malloc(sizeof(struct matrix));
    if (m == NULL) {
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;

    // allocate the data
    m->data = malloc(rows * cols * sizeof(float));
    if (m->data == NULL) {
        free(m);
        return NULL;
    }

    // initialize values
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            if (rows == cols && r == c) {
                m->data[r * cols + c] = 1.0f;
	    } else {
                m->data[r * cols + c] = 0.0f;
            }
        }
    }

    return m;
}

void mat_destroy(Matrix mat) {
    if (!mat) return;
    free(mat->data);
    free(mat);
}

void mat_print(Matrix mat, FILE *stream) {
    if (mat == NULL || stream == NULL) {
        return;
    }

    fprintf(stream, "%zu rows, %zu columns:\n", mat->rows, mat->cols);

    for (size_t r = 0; r < mat->rows; r++) {
        for (size_t c = 0; c < mat->cols; c++) {
            float value = mat->data[r * mat->cols + c];
            fprintf(stream, "%8.3f", value);
        }
        fprintf(stream, "\n");
    }
}



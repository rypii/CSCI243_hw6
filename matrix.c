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

void mat_init(Matrix mat, const float data[]) {
    if (mat == NULL || data == NULL) {
        return;
    }

    size_t total = mat->rows * mat->cols;
    for (size_t i = 0; i < total; i++) {
        mat->data[i] = data[i];
    }
}

Status mat_get_cell(Matrix mat, float *data, size_t row, size_t col) {
    if (mat == NULL || data == NULL) {
        return FAILURE;
    }

    // Bounds check (1-based API)
    if (row < 1 || row > mat->rows || col < 1 || col > mat->cols) {
        return FAILURE;
    }

    // Convert to 0-based indexing internally
    size_t index = (row - 1) * mat->cols + (col - 1);
    *data = mat->data[index];

    return SUCCESS;
}

Status mat_set_cell(Matrix mat, float value, size_t row, size_t col) {
    if (mat == NULL) {
        return FAILURE;
    }

    // Bounds check
    if (row < 1 || row > mat->rows || col < 1 || col > mat->cols) {
        return FAILURE;
    }

    // Convert to 0-based indexing
    size_t index = (row - 1) * mat->cols + (col - 1);
    mat->data[index] = value;

    return SUCCESS;
}

bool mat_equals(const Matrix m1, const Matrix m2) {
    if (m1 == NULL || m2 == NULL) {
        return false;
    }

    if (m1->rows != m2->rows || m1->cols != m2->cols) {
        return false;
    }

    size_t total = m1->rows * m1->cols;
    for (size_t i = 0; i < total; i++) {
        if (m1->data[i] != m2->data[i]) {
            return false;
        }
    }

    return true;
}

Matrix mat_duplicate(const Matrix mat) {
    if (mat == NULL) {
        return NULL;
    }

    Matrix copy = mat_create(mat->rows, mat->cols);
    if (copy == NULL) {
        return NULL;
    }

    size_t total = mat->rows * mat->cols;
    for (size_t i = 0; i < total; i++) {
        copy->data[i] = mat->data[i];
    }

    return copy;
}


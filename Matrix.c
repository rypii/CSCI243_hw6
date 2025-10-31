#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Matrix.h"

struct matrix_st {
    size_t rows;
    size_t cols;
    float *data;   
};

Matrix mat_create(size_t rows, size_t cols) {
    // allocate the struct
    Matrix m = malloc(sizeof(struct matrix_st));
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
        return BadRowNumber; // generic failure, header gives no NULL error type
    }

    if (row < 1 || row > mat->rows) {
        return BadRowNumber;
    }

    if (col < 1 || col > mat->cols) {
        return BadColNumber;
    }

    size_t index = (row - 1) * mat->cols + (col - 1);
    *data = mat->data[index];

    return Success;
}

Status mat_get_row(const Matrix mat, float data[], size_t row) {
    if (mat == NULL || data == NULL) {
        return BadRowNumber; // no general NULL error, so pick row fail
    }

    if (row < 1 || row > mat->rows) {
        return BadRowNumber;
    }

    // convert to 0-based
    size_t r = row - 1;

    for (size_t c = 0; c < mat->cols; c++) {
        data[c] = mat->data[r * mat->cols + c];
    }

    return Success;
}


Status mat_set_cell(Matrix mat, float value, size_t row, size_t col) {
    if (mat == NULL) {
        return BadRowNumber;
    }

    if (row < 1 || row > mat->rows) {
        return BadRowNumber;
    }

    if (col < 1 || col > mat->cols) {
        return BadColNumber;
    }

    size_t index = (row - 1) * mat->cols + (col - 1);
    mat->data[index] = value;

    return Success;
}

Status mat_set_row(Matrix mat, const float data[], size_t row) {
    if (mat == NULL || data == NULL) {
        return BadRowNumber;
    }

    if (row < 1 || row > mat->rows) {
        return BadRowNumber;
    }

    size_t r = row - 1;

    for (size_t c = 0; c < mat->cols; c++) {
        mat->data[r * mat->cols + c] = data[c];
    }

    return Success;
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

void mat_scalar_mult(Matrix mat, float data) {
    if (mat == NULL) {
        return;
    }

    size_t total = mat->rows * mat->cols;
    for (size_t i = 0; i < total; i++) {
        mat->data[i] *= data;
    }
}


Matrix mat_mult(const Matrix m1, const Matrix m2) {
    if (m1 == NULL || m2 == NULL) {
        return NULL;
    }

    // Dimension check: m1 columns must equal m2 rows
    if (m1->cols != m2->rows) {
        return NULL;
    }

    // Create result matrix (initialized to zero)
    Matrix result = mat_create(m1->rows, m2->cols);
    if (result == NULL) {
        return NULL;
    }

    // Matrix multiplication
    for (size_t i = 0; i < m1->rows; i++) {
        for (size_t j = 0; j < m2->cols; j++) {
            float sum = 0.0f;
            for (size_t k = 0; k < m1->cols; k++) {
                sum += m1->data[i * m1->cols + k] * m2->data[k * m2->cols + j];
            }
            result->data[i * result->cols + j] = sum;
        }
    }

    return result;
}

Matrix mat_transpose(const Matrix mat) {
    if (mat == NULL) {
        return NULL;
    }

    // Create a matrix with flipped dimensions
    Matrix result = mat_create(mat->cols, mat->rows);
    if (result == NULL) {
        return NULL;
    }

    for (size_t r = 0; r < mat->rows; r++) {
        for (size_t c = 0; c < mat->cols; c++) {
            result->data[c * result->cols + r] = mat->data[r * mat->cols + c];
        }
    }

    return result;
}

void mat_print(const Matrix mat, FILE *stream) {
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



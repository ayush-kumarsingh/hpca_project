#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2048 // Change this to 8192 for larger matrices

// Function to allocate memory for a matrix
double** allocate_matrix() {
    double** matrix = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        matrix[i] = (double*)malloc(N * sizeof(double));
    }
    return matrix;
}

// Function to free allocated matrix
void free_matrix(double** matrix) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void multiply_matrices(double** A, double** B, double** C) {
    // Initialize the result matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0; // Initialize C to zero
        }
    }

    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void initialize_matrices(double** A, double** B) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)(rand() % 10); // Random values between 0 and 9
            B[i][j] = (double)(rand() % 10);
        }
    }
}

int main() {
    srand(time(NULL)); // Seed for random number generation

    // Dynamically allocate matrices A, B, and C
    double** A = allocate_matrix();
    double** B = allocate_matrix();
    double** C = allocate_matrix();

    // Initialize matrices A and B
    initialize_matrices(A, B);

    // Multiply matrices A and B, store result in C
    multiply_matrices(A, B, C);

    // Optionally print a portion of the resulting matrix (for testing)
    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         printf("%0.1f ", C[i][j]);
    //     }
    //     printf("\n");
    // }

    // Free allocated memory
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return 0;
}


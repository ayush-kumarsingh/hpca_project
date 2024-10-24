#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2048 // change this to 8192
#define TILE_SIZE 64 // Tile size (was B, renamed to TILE_SIZE)

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

// Blocked matrix multiplication function
void multiply_matrices_blocked(double** A, double** B_matrix, double** C) {
    // Initialize result matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
        }
    }

    // Perform blocked multiplication
    for (int i = 0; i < N; i += TILE_SIZE) {         // Outer loop over rows of A (block level)
        for (int j = 0; j < N; j += TILE_SIZE) {     // Outer loop over columns of B (block level)
            for (int k = 0; k < N; k += TILE_SIZE) { // Outer loop over columns of A / rows of B (block level)
                // Multiply the tiles
                for (int ii = i; ii < i + TILE_SIZE && ii < N; ii++) {
                    for (int jj = j; jj < j + TILE_SIZE && jj < N; jj++) {
                        for (int kk = k; kk < k + TILE_SIZE && kk < N; kk++) {
                            C[ii][jj] += A[ii][kk] * B_matrix[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

// Initialize matrices with random values
void initialize_matrices(double** A, double** B_matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)(rand() % 10); // Random values between 0 and 9
            B_matrix[i][j] = (double)(rand() % 10);
        }
    }
}

int main() {
    srand(time(NULL)); // Seed for random number generation

    // Dynamically allocate matrices A, B_matrix, and C
    double** A = allocate_matrix();
    double** B_matrix = allocate_matrix();  // Renamed from B to B_matrix to avoid conflict
    double** C = allocate_matrix();

    // Initialize matrices A and B_matrix
    initialize_matrices(A, B_matrix);

    // Multiply matrices A and B_matrix, store result in C using blocked method
    multiply_matrices_blocked(A, B_matrix, C);

    // Free allocated memory
    free_matrix(A);
    free_matrix(B_matrix);
    free_matrix(C);

    return 0;
}


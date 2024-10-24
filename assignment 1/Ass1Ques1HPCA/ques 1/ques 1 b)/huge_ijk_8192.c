#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define N 8192  // change this to 2048
#define HUGE_PAGE_SIZE (2 * 1024 * 1024)  // 2MB page size

// Function to allocate memory for a matrix using mmap with huge pages
double** allocate_huge_page_matrix() {
    // Try to allocate memory for the matrix using huge pages
    void* addr = mmap(NULL, N * N * sizeof(double), PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    
    if (addr == MAP_FAILED) {
        perror("mmap with huge pages failed");
        exit(EXIT_FAILURE);  // Exit if huge pages allocation fails
    }

    // Convert to a 2D array pointer (use row-major order)
    double** matrix = (double**)malloc(N * sizeof(double*));
    if (!matrix) {
        perror("malloc failed");
        munmap(addr, N * N * sizeof(double));  // Cleanup before exit
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < N; i++) {
        matrix[i] = (double*)((char*)addr + i * N * sizeof(double));
    }

    return matrix;
}

// Function to free allocated matrix
void free_huge_page_matrix(double** matrix) {
    if (matrix) {
        munmap(matrix[0], N * N * sizeof(double));  // Unmap the huge pages
        free(matrix);  // Free the matrix pointer array
    }
}

// Function to multiply matrices
void multiply_matrices(double** A, double** B, double** C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;  // Initialize the result matrix
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to initialize matrices with random values
void initialize_matrices(double** A, double** B) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)(rand() % 10);  // Random values between 0 and 9
            B[i][j] = (double)(rand() % 10);
        }
    }
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    // Allocate matrices A, B, and C using huge pages only
    double** A = allocate_huge_page_matrix();
    double** B = allocate_huge_page_matrix();
    double** C = allocate_huge_page_matrix();

    // Initialize matrices A and B
    initialize_matrices(A, B);

    // Multiply matrices A and B, store result in C
    multiply_matrices(A, B, C);

    // Free allocated memory
    free_huge_page_matrix(A);
    free_huge_page_matrix(B);
    free_huge_page_matrix(C);

    return 0;
}


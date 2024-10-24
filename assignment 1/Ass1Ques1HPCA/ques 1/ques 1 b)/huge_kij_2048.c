#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define N 8192 // change this to 2048 
#define HUGE_PAGE_SIZE (2 * 1024 * 1024) // 2 MB

// Function to allocate memory for a matrix using mmap with huge pages
double** allocate_matrix_mmap() {
    // Allocate memory for the row pointers
    double** matrix = (double**)malloc(N * sizeof(double*));
    if (!matrix) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Allocate a contiguous block of memory for the entire matrix
    size_t total_size = N * N * sizeof(double);
    size_t huge_page_size = HUGE_PAGE_SIZE;

    // Ensure the total size is a multiple of huge page size
    if (total_size % huge_page_size != 0) {
        total_size += huge_page_size - (total_size % huge_page_size);
    }

    // Map the entire matrix in one huge page allocation
    double* data = (double*)mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    if (data == MAP_FAILED) {
        perror("mmap for matrix data");
        free(matrix);
        exit(EXIT_FAILURE);
    }

    // Assign row pointers
    for (int i = 0; i < N; i++) {
        matrix[i] = &data[i * N]; // Point to the beginning of each row
    }

    return matrix;
}

// Function to free the mmap-allocated matrix
void free_matrix_mmap(double** matrix) {
    if (matrix) {
        // Assuming the first pointer in the matrix points to the mmap'd data
        munmap(matrix[0], N * N * sizeof(double)); // Free the entire data block
        free(matrix); // Free the pointer array
    }
}

// Function to multiply matrices
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

// Function to initialize matrices with random values
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

    // Allocate matrices A, B, and C using mmap
    double** A = allocate_matrix_mmap();
    double** B = allocate_matrix_mmap();
    double** C = allocate_matrix_mmap();

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

    // Free the mmap-allocated memory
    free_matrix_mmap(A);
    free_matrix_mmap(B);
    free_matrix_mmap(C);

    return 0;
}


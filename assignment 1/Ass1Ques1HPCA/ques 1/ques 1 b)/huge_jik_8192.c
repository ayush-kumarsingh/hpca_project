#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define N 8192 // change this to 2048
#define HUGE_PAGE_SIZE (2 * 1024 * 1024)

// Function to align a given size to the nearest multiple of HUGE_PAGE_SIZE
size_t align_to_huge_page_size(size_t size) {
    if (size % HUGE_PAGE_SIZE != 0) {
        size += HUGE_PAGE_SIZE - (size % HUGE_PAGE_SIZE);
    }
    return size;
}

// Function to allocate memory for a matrix using mmap with huge pages
double** allocate_matrix() {
    // Align the size for matrix rows to the huge page size
    size_t row_pointer_size = align_to_huge_page_size(sizeof(double*) * N);

    // Allocate memory for matrix row pointers
    double** matrix = (double**)mmap(NULL, row_pointer_size, PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    if (matrix == MAP_FAILED) {
        perror("mmap for matrix rows");
        exit(EXIT_FAILURE);
    }

    // Total number of doubles in the matrix
    size_t total_doubles = N * N;

    // Calculate the total size needed for the data block and align it to the huge page size
    size_t total_size = align_to_huge_page_size(total_doubles * sizeof(double));

    // Allocate memory for the entire matrix in one or more huge pages
    double* data = (double*)mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
    if (data == MAP_FAILED) {
        perror("mmap for matrix data");
        // Free the previously allocated row pointers before exiting
        munmap(matrix, row_pointer_size);
        exit(EXIT_FAILURE);
    }

    // Assign pointers for each row to the correct location in the huge page data block
    for (int i = 0; i < N; i++) {
        matrix[i] = &data[i * N];
    }

    return matrix;
}

// Function to free allocated matrix
void free_matrix(double** matrix) {
    if (matrix) {
        size_t total_doubles = N * N;
        size_t total_size = align_to_huge_page_size(total_doubles * sizeof(double));
        size_t row_pointer_size = align_to_huge_page_size(sizeof(double*) * N);

        // Free the data block and row pointers
        munmap(matrix[0], total_size); // Free the entire matrix data
        munmap(matrix, row_pointer_size); // Free the array of row pointers
    }
}

void multiply_matrices(double** A, double** B, double** C) {
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            C[i][j] = 0; // Initialize the result matrix
            for (int k = 0; k < N; k++) {
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

    // Measure execution time
    clock_t start_time = clock();

    // Multiply matrices A and B, store result in C
    multiply_matrices(A, B, C);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    // Free allocated memory
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return 0;
}


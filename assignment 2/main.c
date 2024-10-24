//old function without the huge pages
//#include "work.h"

//#include <stdio.h>
//#include <stdlib.h>

//int main(int argc, char *argv[]) {
//  if (argc != 2) {
//    fprintf(stderr, "Usage: main <last 5 digits of your reg. no>\n");
//    return EXIT_FAILURE;
//  }
//  work_init(atoi(argv[1]));

//  // Put your changes here

//  if (work_run() == 0) {
//    printf("Work completed successfully\n");
//  }

//  return 0;
//}








//new function to allocate the huge pages

#include "work.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>  // For mmap
#include <errno.h>     // For errno
#include <string.h>    // For strerror

#define MAX_PAGES 8           // Maximum number of large pages
#define LARGE_PAGE_SIZE (2 * 1024 * 1024) // Size of a huge page in bytes

// Hardcoded large page addresses ----- Important please hardcode it before running, hardcore it from the file largepages.txt
static unsigned long large_page_addresses[MAX_PAGES] = {
    0x2854de00000UL, // Address 1
    0x28579c00000UL, // Address 2
    0x28550800000UL, // Address 3
    0x28549a00000UL, // Address 4
    0x2857b400000UL, // Address 5
    0x2854ac00000UL, // Address 6
    0x28578e00000UL, // Address 7
    0x28552800000UL  // Address 8
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: main 24851\n");
        return EXIT_FAILURE;
    }
    
    // Initialize work with registration number
    work_init(atoi(argv[1]));

    // Put your changes here
    // Allocate large pages using mmap()
    for (int i = 0; i < MAX_PAGES; i++) {
        void *addr = (void *)large_page_addresses[i];

        // Use mmap() to map 2MB huge pages at the specified virtual addresses
        void *result = mmap(addr, LARGE_PAGE_SIZE, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_FIXED, -1, 0);

        if (result == MAP_FAILED) {
            fprintf(stderr, "Error allocating huge page at address %lx: %s\n", large_page_addresses[i], strerror(errno));
            return EXIT_FAILURE;
        } else {
            printf("Mapped 2MB huge page at address %p\n", result);
        }
    }

    // Run the main work function
    if (work_run() == 0) {
        printf("Work completed successfully\n");
    }

    return 0;
}

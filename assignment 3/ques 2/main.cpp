#include <iostream>
#include <stdlib.h>
#include "work.h"
#include <pthread.h>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <sys/syscall.h> 
#include <unistd.h>  

#define MAX_THREADS 3

int32_t thread_core_map[MAX_THREADS];
int data_loaded = 0;
// implement the functions here.
void start_monitoring_for_thread(uint32_t threadIdx, pid_t tid, pthread_t handle) {
    std::string command = "perf stat -p " +
                        std::to_string(tid) +
                        " --output=perf_report_thread_" + std::to_string(threadIdx) + ".txt &";
    
    if (system(command.c_str()) == -1)
        std::cerr << "Failed to start monitoring for thread " << threadIdx << std::endl;
    else {
        std::cout << "Starting monitoring for thread " << threadIdx << " with TID: " << tid << std::endl;
    }
}



void stop_monitoring_for_thread(uint32_t threadIdx) {
    std::string command = "pkill -SIGINT perf";
    
    if (system(command.c_str()) == -1)
        std::cerr << "Failed to stop monitoring for thread " << threadIdx << std::endl;
    else {
        std::cout << "Stopped monitoring for thread " << threadIdx << std::endl;
    }
}

void load_core_assignments() {
    // Initialize all entries to -1
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_core_map[i] = -1;
    }

    FILE *file = fopen("core_assignment.txt", "r");
    if (file == NULL) {
        perror("Failed to open core_assignment.txt");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        uint32_t thread;
        int32_t core;

        // Parse the line in the format "Thread X: Core Y"
        if (sscanf(line, "Thread %u: Core %d", &thread, &core) == 2) {
            if (thread < MAX_THREADS) {  // Ensure within array bounds
                // Store the parsed core in the map for this thread
                thread_core_map[thread] = core;
            }
        }
    }

    fclose(file);
    data_loaded = 1;  // Set the flag to indicate data is loaded
}

// Main function to analyze and assign cores
void analyze_threads_perf() {
    int result = system("python3 analyze_threads_perf.py");
    if (result == 0) {
        std::cout << "Performance analysis completed successfully.\n";
    } else {
        std::cerr << "Error: Performance analysis script failed.\n";
    }
}
  

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t get_thread_affinity(uint32_t threadIdx) {
   int32_t core_assignment = -1;
  if (!data_loaded) {
        load_core_assignments();
    }

    // Check if threadIdx is within bounds
    if (threadIdx < MAX_THREADS) {
        core_assignment = thread_core_map[threadIdx];
        std::cout << "\nThread " << threadIdx  << " is scheduled on core: " << core_assignment<< std::endl;
    }
    return core_assignment;
}


int main(int argc, char **argv)
{
  // DO NOT MODIFY THE FOLLOWING BLOCK.
  // =================================================
  if (argc != 2)
  {
    printf("Usage: %s <seed>\n", argv[0]);
    return 1;
  }
  int sr_no = atoi(argv[1]);
  int num_threads = work_init(sr_no);
  // =================================================
  // Modify below this
  
  std::cout << "The number of threads: " << num_threads << std::endl;

  // Modify above this
  // DO NOT MODIFY THE FOLLOWING BLOCK.
  // =================================================
  work_start_monitoring();
  // =================================================
  // Modify below this
  // Your code / script for analysis goes here.
  analyze_threads_perf();
  // Modify above this
  // DO NOT MODIFY THE FOLLOWING BLOCK.
  work_run();
  // =================================================
  // Modify below this

  return 0;
}
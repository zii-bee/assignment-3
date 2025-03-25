#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "array_utils.h"
#include "thread_analysis.h"
#include "single_thread_analysis.h"

int main(int argc, char* argv[]) {
    // Default values
    int array_size = 100000;
    int num_threads = 4;

    // Parse command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "s:t:")) != -1) {
        switch (opt) {
            case 's':
                array_size = atoi(optarg);
                break;
            case 't':
                num_threads = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-s array_size] [-t num_threads]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Generate random array
    int* array = generate_random_array(array_size, 0, 999);

    printf("Statistical Analysis Benchmark\n");
    printf("Array Size: %d\n", array_size);
    printf("Number of Threads: %d\n\n", num_threads);

    // Perform multi-threaded analysis
    perform_multithreaded_analysis(array, array_size, num_threads);

    // Perform single-threaded analysis
    perform_single_threaded_analysis(array, array_size);

    // Clean up
    free_array(array);

    return 0;
}
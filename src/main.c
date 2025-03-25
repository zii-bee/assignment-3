#include <stdio.h>
#include <stdlib.h>
#include "array_utils.h"
#include "thread_analysis.h"
#include "single_thread_analysis.h"

int main() {
    int array_size, num_threads;

    // Prompt for array size
    printf("Enter the size of the array: ");
    scanf("%d", &array_size);

    // Prompt for number of threads
    printf("Enter the number of threads to use: ");
    scanf("%d", &num_threads);

    // Generate random array
    int* array = generate_random_array(array_size, 0, 999);
    printf("Generated an array of size %d with random integers.\n\n", array_size);

    // Store execution times for performance comparison
    double multi_time, single_time;

    // Perform multi-threaded analysis
    multi_time = perform_multithreaded_analysis(array, array_size, num_threads);

    // Perform single-threaded analysis
    single_time = perform_single_threaded_analysis(array, array_size);

    // Calculate performance improvement
    double performance_improvement = ((single_time - multi_time) / single_time) * 100.0;

    // Print performance comparison
    printf("\nPerformance Comparison:\n");
    printf("Multithreaded provided a %.2f%% speed improvement.\n", performance_improvement);

    // Clean up
    free_array(array);

    return 0;
}
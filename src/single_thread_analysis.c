#include <stdio.h>
#include <sys/time.h>
#include <limits.h>
#include "single_thread_analysis.h"

double perform_single_threaded_analysis(int* array, int size) {
    struct timeval start, end;

    // Start computation time measurement
    gettimeofday(&start, NULL);

    // Initialize single-threaded statistics
    long long sum = 0;
    int min = INT_MAX, max = INT_MIN, even_count = 0;

    // Single pass through the array
    for (int i = 0; i < size; i++) {
        int current = array[i];
        sum += current;
        min = (current < min) ? current : min;
        max = (current > max) ? current : max;
        even_count += (current % 2 == 0);
    }

    // End computation time measurement
    gettimeofday(&end, NULL);

    // Calculate average
    double avg = (double)sum / size;

    // Print Single-threaded Calculation Results header
    printf("Single-threaded Calculation Results:\n");
    printf("Overall average: %.2f\n", avg);
    printf("Global minimum: %d\n", min);
    printf("Global maximum: %d\n", max);
    printf("Total count of even numbers: %d\n", even_count);

    // Calculate execution time
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double execution_time = seconds + microseconds * 1e-6;

    printf("Execution time (single-threaded): %.6f seconds\n", execution_time);

    return execution_time;
}
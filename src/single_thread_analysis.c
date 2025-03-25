#include <stdio.h>
#include <sys/time.h>
#include "single_thread_analysis.h"

void perform_single_threaded_analysis(int* array, int size) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Initialize single-threaded statistics
    int sum = 0, min = array[0], max = array[0], even_count = 0;

    // Single pass through the array
    for (int i = 0; i < size; i++) {
        int current = array[i];
        sum += current;
        min = (current < min) ? current : min;
        max = (current > max) ? current : max;
        even_count += (current % 2 == 0);
    }

    // Calculate average
    double avg = (double)sum / size;

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double execution_time = seconds + microseconds * 1e-6;

    // Display single-threaded results
    printf("\nSingle-threaded Analysis Results:\n");
    printf("Average: %.2f\n", avg);
    printf("Minimum: %d\n", min);
    printf("Maximum: %d\n", max);
    printf("Even Count: %d\n", even_count);
    printf("Execution Time: %.6f seconds\n", execution_time);
}
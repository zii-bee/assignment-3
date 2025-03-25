#include <stdio.h>
#include <sys/time.h>
#include "single_thread_analysis.h"

double perform_single_threaded_analysis(int* array, int size) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // initialize single-threaded statistics
    long sum = 0, min = array[0], max = array[0], even_count = 0;

    // single pass through the array (computation)
    for (int i = 0; i < size; i++) {
        int current = array[i];
        sum += current;
        min = (current < min) ? current : min;
        max = (current > max) ? current : max;
        even_count += (current % 2 == 0);
    }

    // calculate average
    double avg = (double)sum / size;

    // end timing immediately after computation
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double execution_time = seconds + microseconds * 1e-6;

    // output single-threaded results
    printf("Single-threaded Calculation Results:\n");
    printf("Overall average: %.2f\n", avg);
    printf("Global minimum: %ld\n", min);
    printf("Global maximum: %ld\n", max);
    printf("Total count of even numbers: %ld\n", even_count);
    printf("Execution time (single-threaded): %.6f seconds\n", execution_time);

    return execution_time;
}

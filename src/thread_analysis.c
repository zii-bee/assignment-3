#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <float.h>
#include "thread_analysis.h"
#include <limits.h>

// Thread function for statistical analysis
void* thread_function(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    SegmentStats* stats = thread_args->result;

    // Initialize thread statistics
    stats->sum = 0;
    stats->min = INT_MAX;
    stats->max = 0;
    stats->even_count = 0;

    // Process assigned segment
    for (int i = thread_args->start; i < thread_args->end; i++) {
        int current = thread_args->array[i];
        stats->sum += current;
        stats->min = (current < stats->min) ? current : stats->min;
        stats->max = (current > stats->max) ? current : stats->max;
        stats->even_count += (current % 2 == 0);
    }

    return NULL;
}

double perform_multithreaded_analysis(int* array, int size, int num_threads) {
    struct timeval start, end;

    // Allocate thread and thread argument arrays
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs* thread_args = malloc(num_threads * sizeof(ThreadArgs));
    SegmentStats* thread_stats = malloc(num_threads * sizeof(SegmentStats));

    // Distribute array segments
    int segment_size = size / num_threads;
    int remainder = size % num_threads;
    int current_start = 0;

    // Start computation time measurement
    gettimeofday(&start, NULL);

    // Prepare and start threads
    for (int i = 0; i < num_threads; i++) {
        int current_segment = segment_size + (i < remainder ? 1 : 0);
        thread_args[i].array = array;
        thread_args[i].start = current_start;
        thread_args[i].end = current_start + current_segment;
        thread_args[i].result = &thread_stats[i];

        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        current_start += current_segment;
    }

    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // End computation time measurement
    gettimeofday(&end, NULL);

    // Calculate global results
    long long global_sum = 0;
    int global_min = INT_MAX, global_max = INT_MIN, global_even_count = 0;

    // Print Multithreaded Calculation Results header
    printf("Multithreaded Calculation Results:\n");

    // Print segment results
    printf("Segment Results:\n");
    current_start = 0;
    for (int i = 0; i < num_threads; i++) {
        int current_segment = segment_size + (i < remainder ? 1 : 0);
        printf("Thread %d (elements %d-%d): Sum: %lld, Min: %d, Max: %d, Even count: %d\n", 
               i+1, current_start, current_start + current_segment - 1,
               (long long)thread_stats[i].sum, thread_stats[i].min, 
               thread_stats[i].max, thread_stats[i].even_count);

        global_sum += thread_stats[i].sum;
        global_min = (thread_stats[i].min < global_min) ? thread_stats[i].min : global_min;
        global_max = (thread_stats[i].max > global_max) ? thread_stats[i].max : global_max;
        global_even_count += thread_stats[i].even_count;
        current_start += current_segment;
    }

    // Calculate global average
    double global_avg = (double)global_sum / size;

    // Print global results
    printf("Overall average: %.2f\n", global_avg);
    printf("Global minimum: %d\n", global_min);
    printf("Global maximum: %d\n", global_max);
    printf("Total count of even numbers: %d\n", global_even_count);

    // Calculate execution time
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double execution_time = seconds + microseconds * 1e-6;

    printf("Execution time (multithreaded): %.6f seconds\n", execution_time);

    // Clean up
    free(threads);
    free(thread_args);
    free(thread_stats);

    return execution_time;
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "thread_analysis.h"

// Thread function for statistical analysis
void* thread_function(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    SegmentStats* stats = thread_args->result;

    // Initialize thread statistics
    stats->sum = 0;
    stats->min = thread_args->array[thread_args->start];
    stats->max = thread_args->array[thread_args->start];
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

    // Prepare thread arguments and create threads
    for (int i = 0; i < num_threads; i++) {
        int current_segment = segment_size + (i < remainder ? 1 : 0);
        thread_args[i].array = array;
        thread_args[i].start = current_start;
        thread_args[i].end = current_start + current_segment;
        thread_args[i].result = &thread_stats[i];

        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        current_start += current_segment;
    }

    // start timing
    gettimeofday(&start, NULL);

    // wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Combine thread statistics
    long global_sum = 0, global_min = thread_stats[0].min, global_max = thread_stats[0].max, global_even_count = 0;
    // Allocate memory for segment starts and sizes
    int* seg_starts = malloc(num_threads * sizeof(int));
    int* seg_sizes = malloc(num_threads * sizeof(int));
    current_start = 0;
    // Calculate global statistics
    for (int i = 0; i < num_threads; i++) {
        int current_segment = segment_size + (i < remainder ? 1 : 0);
        seg_starts[i] = current_start;
        seg_sizes[i] = current_segment;

        global_sum += thread_stats[i].sum;
        if (thread_stats[i].min < global_min)
            global_min = thread_stats[i].min;
        if (thread_stats[i].max > global_max)
            global_max = thread_stats[i].max;
        global_even_count += thread_stats[i].even_count;
        current_start += current_segment;
    }

    // Stop timing after all threads have completed
    gettimeofday(&end, NULL);

    // Calculate global average
    double global_avg = (double)global_sum / size;
    
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    // Calculate execution time
    double execution_time = seconds + microseconds * 1e-6;

    // result printing
    printf("Multithreaded Calculation Results:\n");
    printf("Segment Results:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d (elements %d-%d): Sum: %d, Min: %d, Max: %d, Even count: %d\n", 
               i + 1, seg_starts[i], seg_starts[i] + seg_sizes[i] - 1,
               thread_stats[i].sum, thread_stats[i].min, 
               thread_stats[i].max, thread_stats[i].even_count);
    }
    printf("Overall average: %.2f\n", global_avg);
    printf("Global minimum: %ld\n", global_min);
    printf("Global maximum: %ld\n", global_max);
    printf("Total count of even numbers: %ld\n", global_even_count);
    printf("Execution time (multithreaded): %.6f seconds\n\n", execution_time);

    // Clean up
    free(threads);
    free(thread_args);
    free(thread_stats);
    free(seg_starts);
    free(seg_sizes);

    return execution_time;
}

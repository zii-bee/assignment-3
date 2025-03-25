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

void perform_multithreaded_analysis(int* array, int size, int num_threads) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Allocate thread and thread argument arrays
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs* thread_args = malloc(num_threads * sizeof(ThreadArgs));
    SegmentStats* thread_stats = malloc(num_threads * sizeof(SegmentStats));

    // Distribute array segments
    int segment_size = size / num_threads;
    int remainder = size % num_threads;
    int current_start = 0;

    // Prepare threads
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

    // Calculate global results
    int global_sum = 0, global_min = thread_stats[0].min, global_max = thread_stats[0].max, global_even_count = 0;

    // Display thread-specific results
    printf("Thread-Specific Results:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d: Sum=%d, Min=%d, Max=%d, Even Count=%d\n", 
               i, thread_stats[i].sum, thread_stats[i].min, 
               thread_stats[i].max, thread_stats[i].even_count);

        global_sum += thread_stats[i].sum;
        global_min = (thread_stats[i].min < global_min) ? thread_stats[i].min : global_min;
        global_max = (thread_stats[i].max > global_max) ? thread_stats[i].max : global_max;
        global_even_count += thread_stats[i].even_count;
    }

    // Calculate global average
    double global_avg = (double)global_sum / size;

    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double execution_time = seconds + microseconds * 1e-6;

    // Display global results
    printf("\nMulti-threaded Analysis Results:\n");
    printf("Global Average: %.2f\n", global_avg);
    printf("Global Minimum: %d\n", global_min);
    printf("Global Maximum: %d\n", global_max);
    printf("Total Even Count: %d\n", global_even_count);
    printf("Execution Time: %.6f seconds\n", execution_time);

    // Clean up
    free(threads);
    free(thread_args);
    free(thread_stats);
}
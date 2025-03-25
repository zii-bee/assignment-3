#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>
#include "thread_analysis.h"

// Forward declaration of the thread function
void* thread_function(OptimizedThreadArgs* args);

double perform_multithreaded_analysis(int* array, int size, int num_threads) {
    struct timeval start, end;

    // Dynamically adjust thread count based on system capabilities
    num_threads = (num_threads > 0) ? num_threads : 1;
    num_threads = (num_threads > size) ? size : num_threads;

    // Allocate thread argument array with cache-aligned memory
    OptimizedThreadArgs* thread_args = calloc(num_threads, sizeof(OptimizedThreadArgs));
    pthread_t* threads = malloc(num_threads * sizeof(pthread_t));

    // Improved segment distribution
    int segment_size = size / num_threads;
    int remainder = size % num_threads;
    
    // Start timing
    gettimeofday(&start, NULL);

    // Create threads with reduced overhead
    for (int i = 0; i < num_threads; i++) {
        int current_segment = segment_size + (i < remainder ? 1 : 0);
        int current_start = i * segment_size + (i < remainder ? i : remainder);

        thread_args[i].array = array;
        thread_args[i].start = current_start;
        thread_args[i].end = current_start + current_segment;
        
        // Initialize local statistics
        thread_args[i].sum = 0;
        thread_args[i].min = INT_MAX;
        thread_args[i].max = INT_MIN;
        thread_args[i].even_count = 0;

        // Create thread with explicit cast
        pthread_create(&threads[i], NULL, (void*(*)(void*))thread_function, &thread_args[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // End timing
    gettimeofday(&end, NULL);

    // Aggregate results
    long long global_sum = 0;
    int global_min = INT_MAX, global_max = INT_MIN, global_even_count = 0;

    printf("Multithreaded Calculation Results:\n");
    printf("Segment Results:\n");

    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d (elements %d-%d): Sum: %lld, Min: %d, Max: %d, Even count: %d\n", 
               i+1, thread_args[i].start, thread_args[i].end - 1,
               (long long)thread_args[i].sum, thread_args[i].min, 
               thread_args[i].max, thread_args[i].even_count);

        global_sum += thread_args[i].sum;
        global_min = (thread_args[i].min < global_min) ? thread_args[i].min : global_min;
        global_max = (thread_args[i].max > global_max) ? thread_args[i].max : global_max;
        global_even_count += thread_args[i].even_count;
    }

    // Calculate and print results
    double global_avg = (double)global_sum / size;
    printf("Overall average: %.2f\n", global_avg);
    printf("Global minimum: %d\n", global_min);
    printf("Global maximum: %d\n", global_max);
    printf("Total count of even numbers: %d\n", global_even_count);

    // Calculate execution time
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double execution_time = seconds + microseconds * 1e-6;

    printf("Execution time (multithreaded): %.6f seconds\n\n", execution_time);

    // Clean up
    free(threads);
    free(thread_args);

    return execution_time;
}

// Thread function for processing array segment
void* thread_function(OptimizedThreadArgs* args) {
    // Process assigned segment
    for (int i = args->start; i < args->end; i++) {
        int current = args->array[i];
        args->sum += current;
        args->min = (current < args->min) ? current : args->min;
        args->max = (current > args->max) ? current : args->max;
        args->even_count += (current % 2 == 0);
    }
    return NULL;
}
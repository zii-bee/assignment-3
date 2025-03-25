#ifndef THREAD_ANALYSIS_H
#define THREAD_ANALYSIS_H

#include <pthread.h>
#include "array_utils.h"

// Optimized thread argument structure with better cache alignment
typedef struct {
    int* array;
    int start;
    int end;
    long long sum;
    int min;
    int max;
    int even_count;
    char padding[64]; // Assuming 64-byte cache line
} OptimizedThreadArgs;

// Function to perform multithreaded statistical analysis
double perform_multithreaded_analysis(int* array, int size, int num_threads);

#endif // THREAD_ANALYSIS_H
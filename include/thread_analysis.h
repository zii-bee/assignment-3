#ifndef THREAD_ANALYSIS_H
#define THREAD_ANALYSIS_H

#include <pthread.h>
#include "array_utils.h"

// Thread argument structure
typedef struct {
    int* array;
    int start;
    int end;
    SegmentStats* result;
} ThreadArgs;

// Function to perform multithreaded statistical analysis
double perform_multithreaded_analysis(int* array, int size, int num_threads);

#endif // THREAD_ANALYSIS_H
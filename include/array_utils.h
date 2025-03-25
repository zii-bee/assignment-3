#ifndef ARRAY_UTILS_H
#define __DEF

#include <stdlib.h>

// Struct to hold thread segment statistics
typedef struct {
    int sum;
    int min;
    int max;
    int even_count;
} SegmentStats;

// Function to generate random integer array
int* generate_random_array(int size, int min_val, int max_val);

// Function to free dynamically allocated array
void free_array(int* arr);

#endif // ARRAY_UTILS_H
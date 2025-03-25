#include <stdlib.h>
#include <time.h>
#include "array_utils.h"

int* generate_random_array(int size, int min_val, int max_val) {
    // Seed random number generator
    srand(time(NULL));

    // Allocate memory for array
    int* arr = malloc(size * sizeof(int));
    if (arr == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    // Fill array with random integers
    for (int i = 0; i < size; i++) {
        arr[i] = min_val + rand() % (max_val - min_val + 1);
    }

    return arr;
}

void free_array(int* arr) {
    free(arr);
}
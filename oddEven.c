#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100000 // Size of the array to be sorted

void oddEvenSort(int arr[], int n) {
    int phase, i, temp;
    #pragma omp parallel shared(arr, n) private(i, temp, phase)
    {
        for (phase = 0; phase < n; ++phase) {
            if (phase % 2 == 0) { // Even phase
                #pragma omp for
                for (i = 1; i < n; i += 2) {
                    if (arr[i - 1] > arr[i]) {
                        temp = arr[i - 1];
                        arr[i - 1] = arr[i];
                        arr[i] = temp;
                    }
                }
            } else { // Odd phase
                #pragma omp for
                for (i = 1; i < n - 1; i += 2) {
                    if (arr[i] > arr[i + 1]) {
                        temp = arr[i];
                        arr[i] = arr[i + 1];
                        arr[i + 1] = temp;
                    }
                }
            }
        }
    }
}

int main() {
    int arr[SIZE];
    int i;

    // Initialize array with random values
    for (i = 0; i < SIZE; ++i) {
        arr[i] = rand() % 10000;
    }

    // Serial Odd Even Sort
    double serial_start = omp_get_wtime();
    oddEvenSort(arr, SIZE);
    double serial_end = omp_get_wtime();
    printf("Serial Odd Even Sort Time: %f seconds\n", serial_end - serial_start);
    printf("Sorted array (serial):\n");
    for (i = 0; i < SIZE; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Reinitialize array with random values for parallel sorting
    for (i = 0; i < SIZE; ++i) {
        arr[i] = rand() % 10000;
    }

    // Parallel Odd Even Sort
    double parallel_start = omp_get_wtime();
    oddEvenSort(arr, SIZE);
    double parallel_end = omp_get_wtime();
    printf("\nParallel Odd Even Sort Time: %f seconds\n", parallel_end - parallel_start);
    printf("Sorted array (parallel):\n");
    for (i = 0; i < SIZE; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}


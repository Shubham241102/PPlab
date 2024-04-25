#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Swap two elements
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Partition function
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quick Sort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            quickSort(arr, low, pi - 1);
            #pragma omp section
            quickSort(arr, pi + 1, high);
        }
    }
}

// Function to generate random array
void generateArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // generating random numbers between 0 and 999
    }
}

// Function to print array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n = 10000; // Size of the array
    int arr_serial[n], arr_parallel[n];

    // Generating random array
    generateArray(arr_serial, n);
    for (int i = 0; i < n; i++) {
        arr_parallel[i] = arr_serial[i];
    }

    // Serial Quick Sort
    double start_serial = omp_get_wtime();
    quickSort(arr_serial, 0, n - 1);
    double end_serial = omp_get_wtime();
    printf("Serial execution time: %f seconds\n", end_serial - start_serial);
    printf("Sorted array (serial):\n");
    printArray(arr_serial, n);

    // Parallel Quick Sort
    double start_parallel = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        quickSort(arr_parallel, 0, n - 1);
    }
    double end_parallel = omp_get_wtime();
    printf("Parallel execution time: %f seconds\n", end_parallel - start_parallel);
    printf("Sorted array (parallel):\n");
    printArray(arr_parallel, n);

    return 0;
}

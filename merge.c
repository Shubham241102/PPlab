#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int arr[], int left[], int left_size, int right[], int right_size) {
    int i = 0, j = 0, k = 0;
    while (i < left_size && j < right_size) {
        if (left[i] < right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
    while (i < left_size) {
        arr[k++] = left[i++];
    }
    while (j < right_size) {
        arr[k++] = right[j++];
    }
}

void merge_sort(int arr[], int size) {
    if (size <= 1) {
        return;
    }
    int mid = size / 2;
    int left[mid];
    int right[size - mid];
    for (int i = 0; i < mid; i++) {
        left[i] = arr[i];
    }
    for (int i = mid; i < size; i++) {
        right[i - mid] = arr[i];
    }
    merge_sort(left, mid);
    merge_sort(right, size - mid);
    merge(arr, left, mid, right, size - mid);
}

void parallel_merge_sort(int arr[], int size) {
    if (size <= 1) {
        return;
    }
    int mid = size / 2;
    int left[mid];
    int right[size - mid];
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            for (int i = 0; i < mid; i++) {
                left[i] = arr[i];
            }
            parallel_merge_sort(left, mid);
        }
        #pragma omp section
        {
            for (int i = mid; i < size; i++) {
                right[i - mid] = arr[i];
            }
            parallel_merge_sort(right, size - mid);
        }
    }
    merge(arr, left, mid, right, size - mid);
}

int main() {
    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter the elements of the array: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    double start_time, end_time;

    // Serial merge sort
    int serial_arr[n];
    for (int i = 0; i < n; i++) {
        serial_arr[i] = arr[i];
    }
    start_time = omp_get_wtime();
    merge_sort(serial_arr, n);
    end_time = omp_get_wtime();
    printf("Execution time for serial merge sort: %f seconds\n", end_time - start_time);

    // Parallel merge sort
    start_time = omp_get_wtime();
    parallel_merge_sort(arr, n);
    end_time = omp_get_wtime();
    printf("Execution time for parallel merge sort: %f seconds\n", end_time - start_time);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

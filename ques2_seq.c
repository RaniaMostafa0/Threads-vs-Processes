#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000

void selection_sort(int arr[], int n) {
    int i, j, min_idx, temp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

int main() {
    int array[SIZE];
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        array[i] = rand() % 1001;
    }
    clock_t start, end;
    double cputimeused;
    //Selection Sort
    start = clock();
    selection_sort(array, SIZE);
    end = clock();
    cputimeused = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken in sequential(Selection Sort): %f seconds\n", cputimeused);
    //Quick Sort
    start = clock();
    quick_sort(array, 0, SIZE - 1);
    end = clock();
    cputimeused = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken in sequential(Quick Sort): %f seconds\n", cputimeused);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "quick.h"

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(int arr[], int low, int high, int postorder[]) {
    int pivot = postorder[arr[high]];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (postorder[arr[j]] >= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(int *arr, int low, int high, int *postorder) {
    if (low < high) {
        int pi = partition(arr, low, high, postorder);
        quicksort(arr, low, pi - 1, postorder);
        quicksort(arr, pi + 1, high, postorder);
    }
}

void merge(int* arr, int l, int m, int r, int* postorder) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (postorder[L[i]] >= postorder[R[j]]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int* arr, int l, int r, int* postorder) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, postorder);
        mergeSort(arr, m + 1, r, postorder);
        merge(arr, l, m, r, postorder);
    }
}


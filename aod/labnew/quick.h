#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b);
int partition(int arr[], int low, int high, int postorder[]);
void quicksort(int *arr, int low, int high, int *postorder);
void mergeSort(int* arr, int l, int r, int* postorder);
void merge(int* arr, int l, int m, int r, int* postorder);

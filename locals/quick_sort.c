#include <stdio.h>
#include "randomlist.h"
#include <time.h>
#include <sys/time.h>

// Function to swap two elements
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to print the array
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    struct timeval t1, t2;

    initializeRandomList(10); // Inicializa a lista com 10 números aleatórios

    for (int i = 0; i < dim; i++)
    {
        printf("Unsorted data \n");
        printf("%d \n", A[i]);
    }

    gettimeofday(&t1, NULL);

    quickSort(A, 0, dim - 1);

    gettimeofday(&t2, NULL);

    printf("Sorted array: \n");
    printArray(A, dim);

    double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

    printf("%2.5f seconds\n", t_total);
    freeRandomList();
    return 0;
}
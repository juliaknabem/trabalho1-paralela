#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "randomlist.h"

#define INTERVAL 10001  // Each bucket capacity
#define NUM_THREADS 4

struct Node
{
    int data;
    struct Node *next;
};

struct ThreadData
{
    int *arr;
    struct Node **buckets;
    int start;
    int end;
};

void BucketSort(int arr[], int n, int num_buckets);
struct Node *InsertionSort(struct Node *list);
void print(int arr[], int n);
int getBucketIndex(int value, int interval, int num_buckets);

void *ParallelBucketSort(void *data);

pthread_mutex_t mutex;

void BucketSort(int arr[], int n, int num_buckets)
{
    int i;
    int max_value = arr[0];
    int min_value = arr[0];

    // Find the maximum and minimum values in the array
    for (i = 1; i < n; ++i)
    {
        if (arr[i] > max_value)
            max_value = arr[i];
        if (arr[i] < min_value)
            min_value = arr[i];
    }

    struct Node **buckets = (struct Node **)malloc(sizeof(struct Node *) * num_buckets);

    for (i = 0; i < num_buckets; ++i)
    {
        buckets[i] = NULL;
    }

    for (i = 0; i < n; ++i)
    {
        struct Node *current;
        int pos = getBucketIndex(arr[i], INTERVAL, num_buckets);
        current = (struct Node *)malloc(sizeof(struct Node));
        current->data = arr[i];
        current->next = buckets[pos];
        buckets[pos] = current;
    }

    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_THREADS; ++i)
    {
        threadData[i].arr = arr;
        threadData[i].buckets = buckets;
        threadData[i].start = i * (num_buckets / NUM_THREADS);
        threadData[i].end = (i + 1) * (num_buckets / NUM_THREADS);

        pthread_create(&threads[i], NULL, ParallelBucketSort, &threadData[i]);
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    int j = 0;
    for (i = 0; i < num_buckets; ++i)
    {
        struct Node *node = buckets[i];
        while (node)
        {
            arr[j++] = node->data;
            struct Node *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(buckets);
}

void *ParallelBucketSort(void *data)
{
    struct ThreadData *threadData = (struct ThreadData *)data;
    int *arr = threadData->arr;
    struct Node **buckets = threadData->buckets;
    int start = threadData->start;
    int end = threadData->end;

    for (int i = start; i < end; ++i)
    {
        buckets[i] = InsertionSort(buckets[i]);
    }

    return NULL;
}

struct Node *InsertionSort(struct Node *list)
{
    struct Node *nodeList = list;
    if (list == NULL || list->next == NULL)
    {
        return list;
    }

    struct Node *k = list->next;
    nodeList->next = NULL;

    while (k != NULL)
    {
        struct Node *ptr;
        if (nodeList->data > k->data)
        {
            struct Node *tmp = k;
            k = k->next;
            tmp->next = nodeList;
            nodeList = tmp;
            continue;
        }

        for (ptr = nodeList; ptr->next != NULL; ptr = ptr->next)
        {
            if (ptr->next->data > k->data)
                break;
        }

        if (ptr->next != NULL)
        {
            struct Node *tmp = k;
            k = k->next;
            tmp->next = ptr->next;
            ptr->next = tmp;
            continue;
        }
        else
        {
            ptr->next = k;
            k = k->next;
            ptr->next->next = NULL;
            continue;
        }
    }

    return nodeList;
}

int getBucketIndex(int value, int interval, int num_buckets)
{
    return (value / interval);
}

void print(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    int n = 100000;  // Specify the size of the array
    int array[n];

    // Initialize the random list
    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % n + 1; // Generate a random number
    }

    clock_t t0, t1;
    t0 = clock();

    // Perform bucket sort
    BucketSort(array, n, n);

    t1 = clock();
    double elapsed_time = (double)(t1 - t0) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);

    // Print the sorted array
    // printf("Sorted array: ");
    // print(array, n);

    return 0;
}

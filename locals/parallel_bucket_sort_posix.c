#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "randomlist.h"

#define NBUCKET 2  // Number of buckets
#define INTERVAL 30  // Each bucket capacity
#define NUM_THREADS 2

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

void BucketSort(int arr[]);
struct Node *InsertionSort(struct Node *list);
void print(int arr[]);
int getBucketIndex(int value);

void *ParallelBucketSort(void *data);

pthread_mutex_t mutex;

void BucketSort(int arr[])
{
    int i;
    struct Node **buckets = (struct Node **)malloc(sizeof(struct Node *) * NBUCKET);

    for (i = 0; i < NBUCKET; ++i)
    {
        buckets[i] = NULL;
    }

    for (i = 0; i < dim; ++i)
    {
        struct Node *current;
        int pos = getBucketIndex(arr[i]);
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
        threadData[i].start = i * (NBUCKET / NUM_THREADS);
        threadData[i].end = (i + 1) * (NBUCKET / NUM_THREADS);

        pthread_create(&threads[i], NULL, ParallelBucketSort, &threadData[i]);
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    int j = 0;
    for (i = 0; i < NBUCKET; ++i)
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

int getBucketIndex(int value)
{
    return value / INTERVAL;
}

void print(int arr[])
{
    int i;
    for (i = 0; i < dim; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void)
{

    initializeRandomList(30);

    clock_t t0, t1;
    t0 = clock();

    BucketSort(A);

    t1 = clock();
    double elapsed_time = (double)(t1 - t0) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", elapsed_time);

    printf("Sorted data \n");
    for(int i=0;i<dim;i++) {
        printf("%d ",A[i]);
    }

    return 0;
}

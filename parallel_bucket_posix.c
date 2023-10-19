#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NARRAY 8   // Array size
#define NBUCKET 6  // Number of buckets
#define INTERVAL 101  // Each bucket capacity
#define NUM_THREADS 2  // Number of threads

struct Node {
  int data;
  struct Node *next;
};

struct ThreadData {
  int *arr;
  struct Node **buckets;
  int start;
  int end;
};

void BucketSort(int arr[]);
struct Node *InsertionSort(struct Node *list);
void print(int arr[]);
void printBuckets(struct Node *list);
int getBucketIndex(int value);

void *ParallelBucketSort(void *data);

pthread_mutex_t mutex;

// Sorting function
void BucketSort(int arr[]) {
  int i, j;
  struct Node **buckets;

  // Create buckets and allocate memory size
  buckets = (struct Node **)malloc(sizeof(struct Node *) * NBUCKET);

  // Initialize empty buckets
  for (i = 0; i < NBUCKET; ++i) {
    buckets[i] = NULL;
  }

  // Fill the buckets with respective elements
  for (i = 0; i < NARRAY; ++i) {
    struct Node *current;
    int pos = getBucketIndex(arr[i]);
    current = (struct Node *)malloc(sizeof(struct Node));
    current->data = arr[i];
    current->next = buckets[pos];
    buckets[pos] = current;
  }

  // Create threads and sort buckets in parallel
  pthread_t threads[NUM_THREADS];
  struct ThreadData threadData[NUM_THREADS];

  pthread_mutex_init(&mutex, NULL);

  for (i = 0; i < NUM_THREADS; ++i) {
    threadData[i].arr = arr;
    threadData[i].buckets = buckets;
    threadData[i].start = i * (NBUCKET / NUM_THREADS);
    threadData[i].end = (i + 1) * (NBUCKET / NUM_THREADS);

    pthread_create(&threads[i], NULL, ParallelBucketSort, &threadData[i]);
  }

  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);

  // Put sorted elements on arr
  for (j = 0, i = 0; i < NBUCKET; ++i) {
    struct Node *node;
    node = buckets[i];
    while (node) {
      arr[j++] = node->data;
      node = node->next;
    }
  }

  return;
}

void *ParallelBucketSort(void *data) {
  struct ThreadData *threadData = (struct ThreadData *)data;
  int *arr = threadData->arr;
  struct Node **buckets = threadData->buckets;
  int start = threadData->start;
  int end = threadData->end;

  for (int i = start; i < end; ++i) {
    buckets[i] = InsertionSort(buckets[i]);
  }

  return NULL;
}

// Function to sort the elements of each bucket
struct Node *InsertionSort(struct Node *list) {
  struct Node *k, *nodeList;
  if (list == 0 || list->next == 0) {
    return list;
  }

  nodeList = list;
  k = list->next;
  nodeList->next = 0;
  while (k != 0) {
    struct Node *ptr;
    if (nodeList->data > k->data) {
      struct Node *tmp;
      tmp = k;
      k = k->next;
      tmp->next = nodeList;
      nodeList = tmp;
      continue;
    }

    for (ptr = nodeList; ptr->next != 0; ptr = ptr->next) {
      if (ptr->next->data > k->data)
        break;
    }

    if (ptr->next != 0) {
      struct Node *tmp;
      tmp = k;
      k = k->next;
      tmp->next = ptr->next;
      ptr->next = tmp;
      continue;
    } else {
      ptr->next = k;
      k = k->next;
      ptr->next->next = 0;
      continue;
    }
  }
  return nodeList;
}

int getBucketIndex(int value) {
  return value / INTERVAL;
}

void print(int ar[]) {
  int i;
  for (i = 0; i < NARRAY; ++i) {
    printf("%d ", ar[i]);
  }
  printf("\n");
}

// Print buckets
void printBuckets(struct Node *list) {
  struct Node *cur = list;
  while (cur) {
    printf("%d ", cur->data);
    cur = cur->next;
  }
}

// Driver code
int main(void) {
  int array[NARRAY] = {100, 32, 60, 8, 44, 8, 27, 1};

  printf("Initial array: ");
  print(array);
  printf("-------------\n");

  BucketSort(array);
  printf("-------------\n");
  printf("Sorted array: ");
  print(array);
  return 0;
}
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "randomlist.h"


struct bucket {
    int n_elem;
    int index; // [start : n_elem)
    int start; // starting point in B array
};

int cmpfunc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {

    int *B, *temp;
    int n_buckets, i, w, num_threads, workload, b_index;
    struct bucket* buckets; //array of buckets
    double t1; // Timing variable
    float total; //total time
    n_buckets = 3;
    int *tmp;
    // dim = 100;
    // limit = 1000;

    //global buckets
    int global_n_elem[n_buckets]; //number of elements in each bucket
    int global_starting_position[n_buckets]; //starting position in A for each bucket
    memset(global_n_elem, 0, sizeof(int)*n_buckets);
    memset(global_starting_position, 0, sizeof(int)*n_buckets);

    num_threads = n_buckets;
    printf("number of threads %d \n", num_threads);
    omp_set_num_threads(num_threads);
    initializeRandomList(10); // Inicializa a lista com 10 números aleatórios

    for (int i = 0; i < dim; i++) {
        printf("Unsorted data");
        printf("%d\n", A[i]);
    }

    // freeRandomList(); // Libera a memória alocada
   
    buckets = (struct bucket *) calloc(n_buckets*num_threads, sizeof(struct bucket));
    t1 = omp_get_wtime();


#pragma omp parallel
{
    num_threads = omp_get_num_threads();

    int j,k;
    int local_index; // [0 : n_buckets)
    int real_bucket_index; // [0 : n_buckets * num_threads)
    int my_id = omp_get_thread_num();
    workload = dim/num_threads;
	int prevoius_index;

   #pragma omp for private(i,local_index)
    for (i=0; i< dim;i++){
        local_index = A[i]/w;
        if (local_index > n_buckets-1)
                local_index = n_buckets-1;
        real_bucket_index = local_index + my_id*n_buckets;
        buckets[real_bucket_index].n_elem++;
	}   
   
	int local_sum=0;
	for (j=my_id; j< n_buckets*num_threads; j=j+num_threads){
		local_sum += buckets[j].n_elem;
	}
	global_n_elem[my_id]=local_sum;
	
	#pragma omp barrier
	
	#pragma omp master
    {
	 for (j=1; j<n_buckets; j++){
		        global_starting_position[j] = global_starting_position[j-1] + global_n_elem[j-1];	 
				buckets[j].start = buckets[j-1].start + global_n_elem[j-1];
				buckets[j].index = buckets[j-1].index + global_n_elem[j-1];
		}
	}

	#pragma omp barrier
	for (j=my_id+n_buckets; j< n_buckets*num_threads; j=j+num_threads){
		int prevoius_index = j-n_buckets;
		buckets[j].start = buckets[prevoius_index].start + buckets[prevoius_index].n_elem;
		buckets[j].index = buckets[prevoius_index].index + buckets[prevoius_index].n_elem;	
	}
	#pragma omp barrier

    #pragma omp for private(i, b_index)
    for (i=0; i< dim ;i++){
        j = A[i]/w;
        if (j > n_buckets -1)
                j = n_buckets-1;
        k = j + my_id*n_buckets;
        b_index = buckets[k].index++;
        B[b_index] = A[i];
    }

#pragma omp for private(i)
    for(i=0; i<n_buckets; i++)
        qsort(B+global_starting_position[i], global_n_elem[i], sizeof(int), cmpfunc);
}
   total = omp_get_wtime() - t1;
	tmp = A;
	A = B;
	B = tmp;
   printf("Sorting %d elements took %f seconds\n", dim);
   
    printf("A \n");
    for(i=0;i<dim;i++) {
        printf("%d ",A[i]);
    }
    printf("\n");
    
    printf("Sorting %d elements took %f seconds\n", dim,total);

    int sorted = 1;
    for(i=0;i<dim-1;i++) {
        if (A[i] > A[i+1])
            sorted = 0;
    }
    if (!sorted)
        printf("The data is not sorted!!!\n");
}

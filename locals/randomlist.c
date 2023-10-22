#include "randomlist.h"
#include <stdlib.h>

int *A = NULL;
int dim = 0;

void initializeRandomList(int n) {
    A = (int *)malloc(sizeof(int) * n);
    dim = n;
    for (int i = 0; i < n; i++) {
        A[i] = rand(); // Preenche a lista com números aleatórios
    }
}

void freeRandomList() {
    free(A);
}

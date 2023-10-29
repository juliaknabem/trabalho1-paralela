#include "randomlist.h"
#include <stdlib.h>

int *A = NULL;
int dim = 0;

void initializeRandomList(int n) {
    srand(time(NULL));
    A = (int *)malloc(sizeof(int) * n);
    dim = n;
    for (int i = 0; i < n; i++) {
        A[i] = rand() % n + 1; // Preenche a lista com números aleatórios
    }
}

// Números mal distribuídos:
// void initializeRandomList(int n) {
//     srand(time(NULL));
//     A = (int *)malloc(sizeof(int) * n);
//     dim = n;
//     int range = n / 2; // Define o tamanho da "zona de concentração" nas extremidades

//     for (int i = 0; i < n; i++) {
//         if (i < range || i >= n - range) {
//             // Gere números próximos às extremidades com uma probabilidade maior
//             A[i] = rand() % range;
//         } else {
//             // Gere números no centro com uma probabilidade menor
//             A[i] = range + rand() % (n - 2 * range);
//         }
//     }
// }

void freeRandomList() {
    free(A);
}

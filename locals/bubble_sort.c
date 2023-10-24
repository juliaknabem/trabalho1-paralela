#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "randomlist.h"

int main()
{
 int i, aux, contador;
 initializeRandomList(10); // Inicializa a lista com 10 números aleatórios

  for (int i = 0; i < dim; i++) {
      printf("Unsorted data");
      printf("%d\n", A[i]);
  }

  // freeRandomList();
// Algoritmo de ordenação Bubblesort:
 for (contador = 1; contador < dim; contador++) {
   for (i = 0; i < dim - 1; i++) {
     if (A[i] > A[i + 1]) {
       aux = A[i];
       A[i] = A[i + 1];
       A[i + 1] = aux;
     }
   }
 }
printf("\nElementos do array em ordem crescente:\n");
for (i = 0; i < dim; i++) {
 printf("%4d \n", A[i]);
}
printf("\n");
return 0;
}
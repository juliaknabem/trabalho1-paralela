#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "randomlist.h"
#include <time.h>
#include <sys/time.h>

int main()
{
  struct timeval t1, t2;
  int i, aux, contador;
  initializeRandomList(10); // Inicializa a lista com 10 números aleatórios

  for (int i = 0; i < dim; i++)
  {
    printf("Unsorted data");
    printf("%d\n", A[i]);
  }

  gettimeofday(&t1, NULL);

  // Algoritmo de ordenação Bubblesort:
  for (contador = 1; contador < dim; contador++)
  {
    for (i = 0; i < dim - 1; i++)
    {
      if (A[i] > A[i + 1])
      {
        aux = A[i];
        A[i] = A[i + 1];
        A[i + 1] = aux;
      }
    }
  }

  gettimeofday(&t2, NULL);

  double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

  printf("%2.5f seconds\n", t_total);

  printf("\nElementos do array em ordem crescente:\n");
  for (i = 0; i < dim; i++)
  {
    printf("%4d \n", A[i]);
  }
  printf("\n");
  return 0;
}
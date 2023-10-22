// http://www.bosontreinamentos.com.br/programacao-em-linguagem-c/ordenacao-de-arrays-em-c-com-o-metodo-bubblesort/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <sys/time.h>

#define TAM 10000
int main()
{
  struct timeval t1, t2;
  int i, aux, contador;
  int numeros[TAM] = {7, 2, 3, 2, 1};

  gettimeofday(&t1, NULL);

  // Algoritmo de ordenação Bubblesort:
  for (contador = 1; contador < TAM; contador++)
  {
    for (i = 0; i < TAM - 1; i++)
    {
      if (numeros[i] > numeros[i + 1])
      {
        aux = numeros[i];
        numeros[i] = numeros[i + 1];
        numeros[i + 1] = aux;
      }
    }
  }
  printf("\nElementos do array em ordem crescente:\n");

  gettimeofday(&t2, NULL);

  for (i = 0; i < TAM; i++)
  {
    printf("%4d", numeros[i]);
  }
  printf("\n");

  double t_total = (t2.tv_sec - t1.tv_sec) + ((t2.tv_usec - t1.tv_usec) / 1000000.0);

  printf("%2.5f seconds\n", t_total);
  return 0;
}
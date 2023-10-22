#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define TAM 5
int main()
{
 int i, aux, contador;
 int numeros[TAM] = {7, 2, 3, 2, 1};

// Algoritmo de ordenação Bubblesort:
 for (contador = 1; contador < TAM; contador++) {
   for (i = 0; i < TAM - 1; i++) {
     if (numeros[i] > numeros[i + 1]) {
       aux = numeros[i];
       numeros[i] = numeros[i + 1];
       numeros[i + 1] = aux;
     }
   }
 }
printf("\nElementos do array em ordem crescente:\n");
for (i = 0; i < TAM; i++) {
 printf("%4d", numeros[i]);
}
printf("\n");
return 0;
}
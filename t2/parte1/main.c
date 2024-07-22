#include <stdio.h>
#include "fila.h"

int main()
{
    // cria uma fila vazia de inteiros
    printf(">>>cria uma fila vazia de inteiros\n");
    Fila f = fila_cria(sizeof(int));
    fila_imprime(f);
    printf("\n");

    // insere 1 - 5
    printf(">>>insere 1 - 5\n");
    int pdado;
    // insere 1
    pdado = 1;
    fila_insere(f, &pdado);
    // insere 2 - 5
    for (int i = 2; i < 6; i++)
    {
        pdado = i;
        fila_insere(f, &pdado);
    }
    fila_imprime(f);
    printf("\n");

    // desenfilera
    printf(">>>desenfilera\n");
    fila_remove(f, &pdado);
    fila_imprime(f);
    printf(">removido: %d\n\n", pdado);

    // desenfilera denovo
    printf("desenfilera denovo\n");
    fila_remove(f, &pdado);
    fila_imprime(f);
    printf(">removido: %d\n\n", pdado);

    // remove tudo
    printf(">>>remove tudo\n");
    while (!fila_vazia(f))
    {
        fila_remove(f, NULL);
    }
    fila_imprime(f);
    printf("\n");

    // insere 6
    printf(">>>insere 6\n");
    pdado = 6;
    fila_insere(f, &pdado);
    fila_imprime(f);
    printf("\n");

    // insere 7 - 15
    printf(">>>insere 7 - 15\n");
    for (int i = 7; i < 16; i++)
    {
        pdado = i;
        fila_insere(f, &pdado);
    }
    fila_imprime(f);
    printf("\n");

    // inserçã e remoção pré realloc OK



    fila_destroi(f);
    return 0;
}

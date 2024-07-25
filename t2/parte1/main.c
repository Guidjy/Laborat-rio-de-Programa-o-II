#include <stdio.h>
#include "fila.h"

int main()
{
    // cria uma fila vazia de inteiros
    printf(">>>cria uma fila vazia de inteiros\n");
    Fila f = fila_cria(sizeof(int));
    fila_imprime(f);

    // insere 1
    printf("\n>>>insere 1\n");
    int dado = 1;
    fila_insere(f, &dado);
    fila_imprime(f);

    // insere 2 - 5
    printf("\n>>>insere 2 - 5\n");
    for (int i = 2; i < 6; i++)
    {
        dado = i;
        fila_insere(f, &dado);
    }
    fila_imprime(f);

    // desenfilera uma vez
    printf("\n>>>desenfilera\n");
    fila_remove(f, &dado);
    fila_imprime(f);
    printf(">removido: %d\n", dado);

    // desenfilera tudo
    printf("\n>>>desenfilera tudo\n");
    while(!fila_vazia(f))
    {
        fila_remove(f, NULL);
    }
    fila_imprime(f);

    // insere 6 - 15
    printf("\n>>>insere 6 - 15\n");
    for(int i = 6; i < 16; i++)
    {
        dado = i;
        fila_insere(f, &dado);
    }
    fila_imprime(f);

    // tenta inserir um a mais para realocar o vetor
    printf("\n>>>tenta inserir um a mais para realocar o vetor\n");
    dado = 16;
    fila_insere(f, &dado);
    fila_imprime(f);

    // remove 2 para realocar o vetor
    printf("\n>>>remove 2 para realocar o vetor\n");
    fila_remove(f, NULL);
    fila_remove(f, NULL);
    fila_imprime(f);

    // faz um percurso de início a fim da fila
    printf("\n>>>faz um percurso de início a fim da fila\n");
    fila_inicia_percurso(f, 0);
    while(fila_proximo(f, &dado))
    {
        printf("%d > ", dado);
    }

    // faz um percurso de fim a início da fila
    printf("\n\n>>>faz um percurso de fim a início da fila\n");
    fila_inicia_percurso(f, -1);
    while(fila_proximo(f, &dado))
    {
        printf("%d > ", dado);
    }
    printf("\n");
    

    fila_destroi(f);
    return 0;
}

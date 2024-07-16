#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

int main()
{
    // cria um grafo com nós e de arestas com tamanho de float
    Grafo g = grafo_cria(sizeof(int), sizeof(int));

    // insere um nó com valor 5
    int pdado = 5;
    grafo_insere_no(g, &pdado);
    // insere mais 4 nós com valores diversos
    pdado = 10;
    grafo_insere_no(g, &pdado);
    //pdado = 2;
    //grafo_insere_no(g, (void*)&pdado);
    pdado = 3;
    grafo_insere_no(g, &pdado);
    pdado = 7;
    grafo_insere_no(g, &pdado);
    grafo_imprime(g);

    // insere uma aresta 0 - 2 com valor 1
    pdado = 1;
    grafo_altera_valor_aresta(g, 0, 2, &pdado);
    grafo_imprime(g);




    return 0;
}

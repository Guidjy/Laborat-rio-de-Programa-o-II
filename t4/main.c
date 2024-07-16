#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

int main()
{
    // cria um grafo com nós e de arestas com tamanho de int
    printf(">Cria um grafo: \n");
    Grafo g = grafo_cria(sizeof(int), sizeof(int));

    // insere um nó com valor 5
    int pdado = 5;
    grafo_insere_no(g, &pdado);
    // insere mais 4 nós com valores diversos
    pdado = 9;
    grafo_insere_no(g, &pdado);
    pdado = 2;
    grafo_insere_no(g, &pdado);
    pdado = 3;
    grafo_insere_no(g, &pdado);
    pdado = 7;
    grafo_insere_no(g, &pdado);
    grafo_imprime(g);

    // insere uma série de arestas
    // deve ficar:
    // 0(5) -> [2(1), 3(3)]
    // 1(9) -> [0(4)]
    // 2(2) -> [0(1), 2(0)]
    // 3(3) -> [1(5), 2(5), 4(5)]
    // 4(7) -> []
    printf(">insere uma série de arestas\n");
    pdado = 1;
    grafo_altera_valor_aresta(g, 0, 2, &pdado);
    pdado = 3;
    grafo_altera_valor_aresta(g, 0, 3, &pdado);
    pdado = 4;
    grafo_altera_valor_aresta(g, 1, 0, &pdado);
    pdado = 1;
    grafo_altera_valor_aresta(g, 2, 0, &pdado);
    pdado = 0;
    grafo_altera_valor_aresta(g, 2, 2, &pdado);
    pdado = 5;
    grafo_altera_valor_aresta(g, 3, 1, &pdado);
    grafo_altera_valor_aresta(g, 3, 2, &pdado);
    grafo_altera_valor_aresta(g, 3, 4, &pdado);
    grafo_imprime(g);

    // remove o nó 1
    // deve ficar: 
    // 0(5) -> [ 1(1), 2(3) ]
    // 1(2) -> [ 0(1), 1(0) ]
    // 2(3) -> [ 1(5), 3(5) ]
    // 3(7) -> [ ]
    printf(">Remove o nó 1\n");
    grafo_remove_no(g, 1);
    grafo_imprime(g);

    // remove tudo
    printf(">Remove tudo\n");
    for (int i = 0; i < 4; i++)
    {
        grafo_remove_no(g, 0);
    }
    grafo_imprime(g);

    // enche o grafo denovo
    pdado = 5;
    grafo_insere_no(g, &pdado);
    pdado = 9;
    grafo_insere_no(g, &pdado);
    pdado = 3;
    grafo_insere_no(g, &pdado);
    pdado = 7;
    grafo_insere_no(g, &pdado);

    pdado = 1;
    grafo_altera_valor_aresta(g, 0, 1, &pdado);
    pdado = 3;
    grafo_altera_valor_aresta(g, 0, 2, &pdado);
    pdado = 4;
    grafo_altera_valor_aresta(g, 1, 3, &pdado);
    pdado = 3;
    grafo_altera_valor_aresta(g, 2, 0, &pdado);
    pdado = 0;
    grafo_altera_valor_aresta(g, 2, 2, &pdado);
    pdado = 5;
    grafo_altera_valor_aresta(g, 3, 0, &pdado);
    grafo_altera_valor_aresta(g, 3, 1, &pdado);
    grafo_altera_valor_aresta(g, 3, 2, &pdado);
    grafo_imprime(g);

    grafo_valor_no(g, 1, &pdado);
    printf("Valor nó: %d\n", pdado);
    grafo_valor_aresta(g, 0, 2, &pdado);
    printf("Valor aresta: %d\n", pdado);

    // 

    grafo_destroi(g);
    return 0;
}

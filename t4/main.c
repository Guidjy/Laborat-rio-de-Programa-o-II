#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

int main()
{
    // cria um grafo com nós e de arestas com tamanho de int
    printf(">Cria um grafo: \n");
    Grafo g = grafo_cria(sizeof(int), sizeof(int));

    int pdado = 0;
    grafo_insere_no(g, &pdado);
    grafo_insere_no(g, &pdado);
    grafo_insere_no(g, &pdado);
    grafo_insere_no(g, &pdado);
    grafo_altera_valor_aresta(g, 0, 1, &pdado);
    grafo_altera_valor_aresta(g, 1, 2, &pdado);
    grafo_altera_valor_aresta(g, 2, 3, &pdado);
    grafo_imprime(g);
    
    // checa ciclos
    if (grafo_tem_ciclo(g))
    {
        printf("Tem ciclo\n");
    }
    else
    {
        printf("Nao tem ciclo\n");
    }

    //ordem topológica
    Fila ordem_topologica = fila_cria(sizeof(int));
    ordem_topologica = grafo_ordem_topologica(g);
    fila_imprime(ordem_topologica);

    fila_destroi(ordem_topologica);
    grafo_destroi(g);
    return 0;
}

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
    pdado = 5;
    grafo_altera_valor_aresta(g, 0, 1, &pdado);
    pdado = 6;
    grafo_altera_valor_aresta(g, 0, 2, &pdado);
    pdado = 7;
    grafo_altera_valor_aresta(g, 1, 2, &pdado);
    grafo_imprime(g);

    int vizinho = 0;

    // consulta de arestas que partem
    printf("consulta de arestas que partem de 0: \n");
    grafo_arestas_que_partem(g, 0);
    while (grafo_proxima_aresta(g, &vizinho, &pdado))
    {
        printf("Vizinho: %d\n", vizinho);
        printf("Pdado: %d\n", pdado);
    }
    printf("\n");

    // consulta de arestas que chegam
    printf("consulta de arestas que chegam a 2: \n");
    grafo_arestas_que_chegam(g, 2);
    while (grafo_proxima_aresta(g, &vizinho, &pdado))
    {
        printf("Vizinho: %d\n", vizinho);
        printf("Pdado: %d\n", pdado);
    }

    // checa ciclos
    grafo_tem_ciclo(g);

    grafo_destroi(g);
    return 0;
}

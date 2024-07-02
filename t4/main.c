#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "grafo.h"


int main()
{
    // cria um grafo
    Grafo g1 = grafo_cria(sizeof(float), sizeof(float));

    // insere um nó
    float temp = 1.0;
    int id;
    id = grafo_insere_no(g1, &temp);
    temp = 2.0;
    id = grafo_insere_no(g1, &temp);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "grafo.h"


// estrutura de um nó de fila de adjacência 
struct _no_grafo
{
    int no_id;               // indentificação do nó 
    void *no_valor;          // valor associado ao nó (vértice)
    void *aresta_valor;      // valor associado à aresta
    struct _no_grafo *prox;  // ponteiro para o próximo nó na lista de adjacência
};
typedef struct _no_grafo *No_grafo;


// estrutura de um grafo implementado como listas de adjacência
struct _grafo
{
    int n_nos;                       // número de nós no grafo
    int tam_no;                      // tamanho em bytes do valor associado ao nó
    int tam_aresta;                  // tamanho em bytes do valor associado à aresta
    No_grafo *listas_de_adjacencia;  // vetor de listas de adjacência
};
// typedef struct _grafo *Grafo; >>> grafo.h


Grafo grafo_cria(int tam_no, int tam_aresta)
{
    // aloca memória para a estrutura do grafo
    Grafo novo_grafo = (Grafo) malloc(sizeof(struct _grafo));
    assert(novo_grafo != NULL);
    // preenche os descritores
    novo_grafo->n_nos = 0;
    novo_grafo->tam_no = tam_no;
    novo_grafo->tam_aresta = tam_aresta;
    novo_grafo->listas_de_adjacencia = NULL;
    // retorna o grafo criado
    return novo_grafo;    
}


void grafo_destroi(Grafo self)
{
    // TODO:
}


// realoca memória para o vetor de listas de adjacência
static void realoca_vetor_listas_adjacencia(Grafo self)
{
    self->listas_de_adjacencia = (No_grafo*) realloc(self->listas_de_adjacencia, grafo_nnos(self) * sizeof(struct _no_grafo));
    assert(self->listas_de_adjacencia != NULL);
}


int grafo_insere_no(Grafo self, void *pdado)
{
    // 1) insere um nó no grafo, com o dado apontado por pdado
    // 2) retorna o número do novo nó

    // aloca memória para um novo nó
    No_grafo novo_no = (No_grafo) malloc(sizeof(struct _no_grafo));
    assert(novo_no != NULL);

    // descobre a identificação do nó
    // "os nós são identificados por um inteiro que corresponde à ordem em que são inseridos, 
    // com 0 correspondendo ao primeiro."
    novo_no->no_id = grafo_nnos(self);

    // inicializa os ponteiros do nó
    novo_no->no_valor = malloc(self->tam_no);
    assert(novo_no->no_valor != NULL);
    novo_no->aresta_valor = malloc(self->tam_aresta);
    assert(novo_no->aresta_valor != NULL);
    novo_no->prox = NULL;

    // copia o dado de *pdado para o nó
    memmove(novo_no->no_valor, pdado, self->tam_no);

    // aloca mais memória para o vetor de listas de adjacências
    self->n_nos++;
    realoca_vetor_listas_adjacencia(self);
    // insere o novo nó no vetor de listas de adjacências
    self->listas_de_adjacencia[novo_no->no_id] = novo_no;

    // retorna o número do novo nó
    return novo_no->no_id;
}


void grafo_remove_no(Grafo self, int no)
{
    // TODO:
}


void grafo_altera_valor_no(Grafo self, int no, void *pdado)
{
    // TODO:
}


void grafo_valor_no(Grafo self, int no, void *pdado)
{
    // TODO:
}


int grafo_nnos(Grafo self)
{
    return self->n_nos;
}

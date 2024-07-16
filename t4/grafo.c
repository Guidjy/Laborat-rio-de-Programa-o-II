#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"


// estrutura de um nó das listas de adjacência do grafo
struct _no_grafo
{
    int no_id;               // inteiro que indentifica o nó
    void *valor_no;          // ponteiro para o valor associado ao nó
    void *valor_aresta;      // ponteiro para o valor associado à aresta
    struct _no_grafo *prox;  // ponteiro para o próximo nó na lista de adjacência
};
typedef struct _no_grafo *No_grafo;


// estrutura do grafo
struct _grafo
{
    int tam_no;                      // tamanho em bytes do valor associado ao nó
    int tam_aresta;                  // tamanho em bytes do valor associado à aresta
    int cap;                         // capacidade do vetor de listas de adjacência
    int n_nos;                       // número de nós no grafo
    No_grafo *listas_de_adjacencia;  // vetor de listas de adjacência
};
// typedef struct _grafo *Grafo; >>> grafo.h


Grafo grafo_cria(int tam_no, int tam_aresta)
{
    // aloca memória para a estrutura de um grafo
    Grafo novo_grafo = (Grafo) malloc(sizeof(struct _grafo));
    if (novo_grafo == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA A ESTRUTURA DO GRAFO\n");
        return NULL;
    }

    // inicializa os descritores do grafo
    novo_grafo->tam_no = tam_no;
    novo_grafo->tam_aresta = tam_aresta;
    novo_grafo->cap = 10;
    novo_grafo->n_nos = 0;

    // aloca memória para o vetor de listas de adjacência
    novo_grafo->listas_de_adjacencia = (No_grafo*) malloc(novo_grafo->cap * sizeof(No_grafo));
    if (novo_grafo->listas_de_adjacencia == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA O VETOR DE LISTAS DE ADJACÊNCIA DO GRAFO\n");
        free(novo_grafo);
        return NULL;
    }

    return novo_grafo;
}


// Funções de manipulação de nós (vértices)


// cria um nó
static No_grafo no_grafo_cria(Grafo self, int no_id)
{
    // aloca memória para um nó do grafo
    No_grafo novo_no = (No_grafo) malloc(sizeof(struct _no_grafo));
    if (novo_no == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA UM NOVO NÓ DO GRAFO\n");
        return NULL;
    }
    // inicializa os descritores
    novo_no->no_id = no_id;
    novo_no->prox = NULL;

    // aloca memória para os ponteiros de valores
    novo_no->valor_no = malloc(self->tam_no);
    novo_no->valor_aresta = malloc(self->tam_aresta);
    if (novo_no->valor_no == NULL || novo_no->valor_aresta == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA OS PONTEIROS DE VALOR DE UM NÓ DO GRAFO\n");
        free(novo_no);
        return NULL;
    }

    return novo_no;
}


// realoca memória para o vetor de listas de adjacência
static void realoca_vetor(Grafo self)
{
    self->listas_de_adjacencia = (No_grafo*) realloc(self->listas_de_adjacencia, self->cap * sizeof(No_grafo));
    if (self->listas_de_adjacencia == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA O VETOR DE LISTAS DE ADJACÊNCIA\n");
        return;
    }
}


int grafo_insere_no(Grafo self, void *pdado)
{
    // cria um novo nó
    No_grafo novo_no = no_grafo_cria(self, self->n_nos);
    if (novo_no == NULL)
    {
        return -1;
    }
    // atribui o valor de pdado ao nó
    memmove(novo_no->valor_no, pdado, self->tam_no);

    // incrementa o número de nós no grafo
    self->n_nos++;
    // verifica se o vetor de listas de adjacência está cheio
    if (self->n_nos == self->cap)
    {
        // dobra a capacidade do vetor
        self->cap *= 2;
        realoca_vetor(self);
        if (self->listas_de_adjacencia == NULL) return -1;
    }

    // insere o novo nó no vetor de listas de adjacência
    self->listas_de_adjacencia[self->n_nos - 1] = novo_no;

    return novo_no->no_id;
}


void grafo_valor_no(Grafo self, int no, void *pdado)
{
    memmove(pdado, self->listas_de_adjacencia[no]->valor_no, self->tam_no);
}


int grafo_nnos(Grafo self)
{
    return self->n_nos;
}


// Funções de manipulação de arestas (conexões)


void grafo_altera_valor_aresta(Grafo self, int origem, int destino, void *pdado)
{
    // altera o valor da aresta que interliga o nó origem ao nó destino (copia de *pdado)
    // caso a aresta não exista, deve ser criada
    // caso pdado seja NULL, a aresta deve ser removida

    // se pdado for NULL, a aresta deve ser removida
    if (pdado == NULL)
    {
        // percorre a lista de adjacência do nó em origem até achar o nó em destino ou até seu final
        No_grafo p = self->listas_de_adjacencia[origem]->prox;  // prox porque não se remove a cabeça da lista
        No_grafo p_ant = NULL;
        while (p != NULL && p->no_id != destino)
        {
            p_ant = p;
            p = p->prox;
        }
        // se não existir uma aresta entre origem e destino
        if (p == NULL)
        {
            // não faz nada
            return;
        }
        // se a aresta a remover estiver no início da lista
        else if (p_ant == NULL)
        {
            self->listas_de_adjacencia[origem]->prox = p->prox;
            free(p);
        }
        // se a aresta a remover estiver no meio ou final da lista
        else
        {
            p_ant->prox = p->prox;
            free(p);
        }
    }
    // se pdado não for NULL
    else
    {
        // percorre a lista de adjacências do nó em origem
        No_grafo p = self->listas_de_adjacencia[origem];
        No_grafo p_ant = NULL;
        while (p != NULL && p->no_id != destino)
        {
            p_ant = p;
            p = p->prox;
        }
        // caso uma aresta conectando origem e destino não exista, deve ser criada
        if (p == NULL)
        {
            No_grafo novo_no = no_grafo_cria(self, destino);
            if (novo_no == NULL)
            {
                return;
            }
            memmove(novo_no->valor_no, self->listas_de_adjacencia[destino]->valor_no, self->tam_no);
            memmove(novo_no->valor_aresta, pdado, self->tam_aresta);
            p_ant->prox = novo_no;
        }
        // caso ja tenha uma aresta
        else
        {
            memmove(p->valor_aresta, pdado, self->tam_aresta);
        }
    }
}


// algoritmos


void grafo_imprime(Grafo self)
{
    // se o grafo estiver vazio
    if (grafo_nnos(self) == 0)
    {
        printf("[]\n");
        printf("n_nos: %d\n", self->n_nos);
        printf("cap:   %d\n", self->cap);
        printf("\n\n");
        return;
    }
    // percorre o vetor de listas de adjacências
    for (int i = 0; i < grafo_nnos(self); i++)
    {
        printf("%d(%d) -> [ ", self->listas_de_adjacencia[i]->no_id, *(int*)self->listas_de_adjacencia[i]->valor_no);
        // ponteiro para percorrer a lista de adjacências
        No_grafo p = self->listas_de_adjacencia[i]->prox;
        // imprime a lista de adjacências
        if (p == NULL)
        {
            printf("]\n");
        }
        else
        {
            while (p->prox != NULL)
            {
                printf("%d(%d), ", p->no_id, *(int*)p->valor_aresta);
                p = p->prox;
            }
            printf("%d(%d) ]\n", p->no_id, *(int*)p->valor_aresta);
        }
    }
    printf("n_nos: %d\n", self->n_nos);
    printf("cap:   %d\n", self->cap);
    printf("\n\n");
}







int grafo_nnos(Grafo self)
{
    return self->n_nos;
}

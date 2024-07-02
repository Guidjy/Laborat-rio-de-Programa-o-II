#include <stdio.h>
#include <stdlib.h>
#include "fila.h"


// estrutura de um nó da fila de inteiros
struct _no_fila
{
    int info;               // dado armazenado no nó
    struct _no_fila *prox;  // ponteiro para o próximo nó da fila
};
typedef struct _no_fila *No_fila;


// estrutura de um descritor da fila
struct _fila
{
    int n_elem;     // número de nós na fila
    No_fila pri;    // ponteiro para o primeiro elemento da fila
    No_fila ult;    // ponteiro para o último elemento da fila
};
// typedef struct _fila *Fila; >>> fila.h


Fila fila_cria()
{
    // aloca memória para o descritor da fila
    Fila nova_fila = (Fila) malloc(sizeof(struct _fila));
    if (nova_fila == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA A FILA\n");
        return NULL;
    }
    // inicializa os descritores
    nova_fila->n_elem = 0;
    nova_fila->pri = NULL;
    nova_fila->ult = NULL;
    // retorna a fila criada
    return nova_fila;
}


bool fila_vazia(Fila self)
{
    return self->n_elem == 0;
}


void fila_enfilera(Fila self, int num)
{
    // aloca memória para um novo nó
    No_fila novo_no = (No_fila) malloc(sizeof(struct _no_fila));
    if (novo_no == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA NÓ DA FILA\n");
        return;
    }
    novo_no->info = num;
    novo_no->prox = NULL;
    // encadeia o nó no final da fila
    if (fila_vazia(self))
    {
        self->pri = novo_no;
    }
    else
    {
        self->ult->prox = novo_no;
    }
    self->ult = novo_no;
    // incrementa o número ed elementos na fila
    self->n_elem++;
}


int fila_desenfilera(Fila self)
{
    // verifica se a fila está vazia
    if (fila_vazia(self))
    {
        printf("FILA VAZIA, NÃO SE PODE REMOVER ELEMENTOS\n");
        return -1;
    }
    // guarda um ponteiro para o elemento a ser removido (primeiro nó)
    No_fila removido = self->pri;
    // guarda o número armazenado pelo nó a ser removido
    int valor_removido = removido->info;
    // desencadeia o primeiro nó
    self->pri = self->pri->prox;
    // decrementa o número de elementos na fila
    self->n_elem--;
    if (fila_vazia(self))
    {
        self->ult = NULL;
    }
    // libera o nó desencadeado
    free(removido);
    // retorna o valor removido
    return valor_removido;
}


void fila_imprime(Fila self)
{
    if (fila_vazia(self))
    {
        printf("[]\n");
        return;
    }
    printf("[");
    No_fila p;
    for (p = self->pri; p->prox != NULL; p = p->prox)
    {
        printf("%d, ", p->info);
    }
    printf("%d]\n", p->info);
}


void fila_libera(Fila self)
{
    No_fila p = self->pri;
    while (p != NULL)
    {
        No_fila temp = p->prox;
        free(p);
        p = temp;
    }
    free(self);
}

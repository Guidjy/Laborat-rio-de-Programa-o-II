#ifndef _LISTA_H_
#define _LISTA_H_
#include <stdbool.h>

// define o tipo de dado lista
typedef struct _lista *Lista;

// cria uma lista vazia
Lista lista_cria();

// insere um elemento no início de uma lista
// obs: o dado a ser inserido deve ser passado por referência
void lista_insere(Lista self, void *dado, char *tipo);

// remove o elemento no início da lista e retorna um ponteiro para seu dado
void *lista_remove(Lista self);

// insere um elemento no índice pos da lista
void lista_insere_pos(Lista self, void *dado, char *tipo, int pos);

// remove um elemento no índice pos da lista e retorna um ponteiro para seu dado
void *lista_remove_pos(Lista self, int pos);

// enfileira um elemento
void lista_enqueue(Lista self, void *dado, char *tipo);

// desenfilera a fila e retorna um ponteiro para o dado removido
void *lista_dequeue(Lista self);

// insere um elemento no início
void lista_preppend(Lista self, void *dado, char *tipo);

// empilha um elemento
void lista_append(Lista self, void *dado, char *tipo);

// desempilha a pilha e retorna um ponteiro para o dado removido
void *lista_pop(Lista self);

// retorna true se uma lista estiver vazia
bool lista_vazia(Lista self);

// retorna o número de elementos em uma lista
int lista_n_elem(Lista self);

// retorna o dado do nó na posição pos da lista
void *lista_retorna_dado(Lista self, int pos);

// organiza os elementos da lista em ordem crescente
void lista_sort(Lista self);

// imprime uma lista
void lista_imprime(Lista self);

// libera a memória alocada para uma lista e seus nós
void lista_libera(Lista self);

#endif

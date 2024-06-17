// árvore binária de busca de strings para o T3 de Lab II

#ifndef _ABB_H_
#define _ABB_H_

#include <stdbool.h>

// define o tipo árvore como um ponteior para um nó raíz
typedef struct _no_arv *arv;

// cria uma árvore vazia
arv arv_cria();

// retorna true se uma árvore/sub-árvore estiver vazia
bool arv_vazia(arv self);

// insere uma palavra na árvore
void arv_insere(arv self, char *pal);

// remove a palavra passada da árvore, e retorna true se teve sucesso
bool arv_remove(arv self, char *pal);

// retorna a altura de uma árvore ou sub-árvore
int arv_altura(arv self);

// calcula o fator de equilíbrio de uma árvore
int arv_fator_de_equilibrio(arv self);

// retorna true se um nó for uma folha
bool eh_folha(arv self);

// imprime uma árvore.
// recebe um ponteiro para a raíz e o nível da qual deve começar a imprimir
void arv_imprime(arv raiz, int nivel);

// libera a árvore inteira
void arv_libera(arv self);

#endif

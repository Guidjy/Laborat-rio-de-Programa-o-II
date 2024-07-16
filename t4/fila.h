#ifndef _FILA_H_
#define _FILA_H_

#include <stdbool.h>

typedef struct _fila *Fila;

// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila fila_cria(int tam_do_dado);

void fila_destroi(Fila self);

bool fila_vazia(Fila self);

void fila_remove(Fila self, void *pdado);

void fila_insere(Fila self, void *pdado);

void fila_inicia_percurso(Fila self, int pos_inicial);

bool fila_proximo(Fila self, void *pdado);

#endif // _FILA_H_

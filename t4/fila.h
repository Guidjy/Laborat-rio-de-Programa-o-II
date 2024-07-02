#ifndef _FILA_H_
#define _FILA_H_

#include <stdbool.h>

typedef struct _fila *Fila;

// cria uma fila vazia
Fila fila_cria();

// verifica se uma fila está vazia
bool fila_vazia(Fila self);

// enfilera um dado
void fila_enfilera(Fila self, int num);

// desenfilera a fila e retorna o dado do nó desenfilerado
int fila_desenfilera(Fila self);

// imprime uma fila
void fila_imprime(Fila self);

// libera a memória alocada para uma fila
void fila_libera(Fila self);

#endif

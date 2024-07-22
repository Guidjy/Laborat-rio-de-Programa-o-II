#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"


// estrutura da fila
struct _fila
{
    int cap;           // capacidade máxima da fila
    int n_elem;        // número de elementos na fila
    int tam_dado;      // tamanho em bytes do dado armazenado na fila
    int pos_percurso;  // variável do jogo da cobra
    int pri;           // índice do primeiro elemento da fila
    int fim;           // índice do final da fila
    void *espaco;      // vetor circular com os elementos da fila
};
// typedef struct _fila *Fila; >>> fila.h


// funções que implementam as operações básicas de uma fila


// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila fila_cria(int tam_do_dado)
{
    // aloca memória para a estrutura da fila
    Fila nova_fila = (Fila) malloc(sizeof(struct _fila));
    if (nova_fila == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA A ESTRUTURA DA FILA\n");
        return NULL;
    }
    // inicializa os descritores da fila
    nova_fila->cap = 10;  // inicializa a fila com uma capacidade máxima de 10 elementos
    nova_fila->n_elem = 0;  // fila está vazia
    nova_fila->tam_dado = tam_do_dado;
    nova_fila->pri = 0;
    nova_fila->fim = 0;
    // aloca memória para o vetor da fila
    nova_fila->espaco = malloc(nova_fila->cap * nova_fila->tam_dado);
    if (nova_fila->espaco == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA PARA O VETOR DA FILA\n");
        free(nova_fila);
        return NULL;
    }
    
    return nova_fila;
}


// libera toda a memória alocada para uma fila
void fila_destroi(Fila self)
{
    if (self != NULL)
    {
        if (self->espaco != NULL) free(self->espaco);
        free(self);
    }
}


// diz se a fila está vazia
bool fila_vazia(Fila self)
{
    return self->n_elem == 0;
}


bool fila_cheia(Fila self)
{
    return self->n_elem == self->cap;
}


// calcula o valor do ponteiro para o elemento na posição pos da fila
//   retorna NULL se não existir elemento nessa posição da fila
static void *calcula_ponteiro(Fila self, int pos)
{
  // TODO: suporte a pos negativa
  if (pos < 0)
  {
    pos += self->n_elem;
  }

  // se a posição fornecida não estiver dentro da fila, retorna NULL
  if (pos < 0 || pos >= self->cap) return NULL;
  
  // não conhecemos o tipo do dado do usuário, só o tamanho em bytes.
  // temos um ponteiro para void apontando para o início do vetor.
  // o dado desejado está tantos bytes além do início do vetor.
  // para calcular a posição do dado, temos que converter o ponteiro void
  //   para um ponteiro para dados do tamanho de um byte (char).
  // a posição do dado no vetor é a mesma posição do dado no vetor (isso não
  //   será mais válido com vetor circular).
  int deslocamento_em_bytes = pos * self->tam_dado;
  char *ptr_em_bytes_para_o_inicio = (char *)(self->espaco);
  char *ptr_em_bytes_para_o_dado = ptr_em_bytes_para_o_inicio + deslocamento_em_bytes;
  void *ptr = (void *)ptr_em_bytes_para_o_dado;
  return ptr;
}


// insere o dado apontado por pdado no final da fila
void fila_insere(Fila self, void *pdado)
{
    // verifica se a fila está cheia
    if (fila_cheia(self))
    {
        printf("FILA CHEIA, NÃO SE PODE INSERIR DADOS\n");
        return;
    }
    
    // incrementa o número de elementos, para que a próxima posição seja válida e seu endereço possa ser calculado
    self->n_elem++;
    // calcula a posição onde o dado deve ser colocado
    void *final_da_fila = calcula_ponteiro(self, self->fim);
    // insere o dado no vetor
    memmove(final_da_fila, pdado, self->tam_dado);
    // altera o índice do final da fila, mantendo a implementação circular
    self->fim = (self->fim + 1) % self->cap;
}


// remove o dado no início da fila e, se pdado não for NULL, copia o dado removido para *pdado
void fila_remove(Fila self, void *pdado)
{
    // verifica se a fila está vazia
    if (fila_vazia(self))
    {
        printf("FILA VAZIA, NÃO SE PODE REMOVER DADOS\n");
        return;
    }

    // obtem ponteiro para o primeiro elemento da fila
    void *inicio_da_fila = calcula_ponteiro(self, self->pri);
    // copia o dado do primeiro elemento para pdado
    if (pdado != NULL)
    {
        memmove(pdado, inicio_da_fila, self->tam_dado);
    }

    // altera o índice do primeiro elemento, mantendo a implementação circular
    self->pri = (self->pri + 1) % self->cap;
    // decrementa o número de elementos da fila
    self->n_elem--;
}


void fila_imprime(Fila self)
{
    printf("n_elem: %d\n", self->n_elem);
    printf("cap: %d\n", self->cap);
    printf("pri: %d\n", self->pri);
    printf("fim: %d\n", self->fim);
    printf("vetor: [");
    int *temp = (int*) self->espaco;
    for (int i = 0; i < self->cap; i++)
    {
        printf("%d, ", temp[i]);
    }
    printf("]\n");
    printf("fila:  [");
    for (int i = 0; i < self->n_elem; i++)
    {
        int pv = (self->pri + i) % self->cap;
        printf("%d, ", temp[pv]);
    }
    printf("]\n");
}

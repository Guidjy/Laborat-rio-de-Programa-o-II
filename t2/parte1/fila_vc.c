#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "fila.h"

// número máximo de elementos na fila
#define MAX_ELEM 10


// estrutura da fila
struct _fila
{
    int n_elem;        // número de elementos no vetor
    int cap;           // capacidade máxima do vetor
    int tam_dado;      // tamanho em bytes do dado armazenado no vetor
    int pos_percurso;  // variável do jogo da cobra
    int pri;           // índice do primeiro elemento da fila
    int fim;           // índice do final da fila (não do último elemento)
    void *espaco;      // vetor de dados
};
// typedef struct _fila *Fila; >>> fila.h


// funções que implementam as operações básicas de uma fila


// cria uma fila vazia que suporta dados do tamanho fornecido (em bytes)
Fila fila_cria(int tam_do_dado)
{
    // aloca memória para a estrutura da fila
    Fila nova_fila = (Fila) malloc(sizeof(struct _fila));
    assert(nova_fila != NULL);
    // inicializa os descritores da fila
    nova_fila->n_elem = 0;
    nova_fila->cap = MAX_ELEM;
    nova_fila->tam_dado = tam_do_dado;
    nova_fila->pri = 0;
    nova_fila->fim = 0;
    // aloca memória para o vetor da fila
    nova_fila->espaco = malloc(nova_fila->cap * sizeof(nova_fila->tam_dado));
    assert(nova_fila->espaco != NULL);

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


// diz se a fila está cheia 
static bool fila_cheia(Fila self)
{
    return self->n_elem == self->cap;
}


// retorna um ponteiro para o elemento na posição "pos" da fila (retorna NULL caso não exista)
static void *calcula_ponteiro(Fila self, int pos)
{
  // suporte a pos negativa
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


// aloca mais memória para o vetor da fila
static void realoca_vetor_mais(Fila self)
{
    // a função vai duplicar a quantidade de índices do vetor. Para que a implementação circular seja mantida,
    // deve-se movimentar os dados do vetor. O Primeiro elemento e todos os subsequentes até o final do vetor original
    // vão ser passados para o final do novo vetor.

    // vetor com a nova capacidade
    void *novo_espaco = malloc(self->cap * 2 * self->tam_dado);
    assert(novo_espaco != NULL);

    // move a lista inteira para o início do vetor
    for (int i = 0; i < self->n_elem; i++)
    {
        int pv = (self->pri + i) % self->cap;
        void *origem = calcula_ponteiro(self, pv);
        void *destino = (char*)novo_espaco + i * self->tam_dado;
        assert(origem != NULL && destino != NULL);
        memmove(destino, origem, self->tam_dado);
    }
    // atualiza a capacidade
    self->cap *= 2;
    // altera os índices de pri e fim
    self->pri = 0;
    self->fim = (self->pri + self->n_elem) % self->cap;

    // atualiza o ponteiro do vetor
    free(self->espaco);
    self->espaco = novo_espaco;
}


// insere o dado apontado por pdado no final da fila
void fila_insere(Fila self, void *pdado)
{
    // verifica se a fila está cheia
    if (fila_cheia(self))
    {
        // se estiver, sobra sua capacidade e aloca mais memória para ela
        realoca_vetor_mais(self);
    }

    // incrementa o número de elementos, para que a próxima posição seja válida e seu endereço possa ser calculado
    self->n_elem++;
    // calcula um ponteiro para o final da fila
    void *final_da_fila = calcula_ponteiro(self, self->fim);
    assert(final_da_fila != NULL);

    // copia o dado de pdado para o final da fila
    memmove(final_da_fila, pdado, self->tam_dado);

    // altera a posição do final da fila, de acordo com a implementação circular
    self->fim = (self->fim + 1) % self->cap;
}


// desaloca memória para o vetor da fila
static void realoca_vetor_menos(Fila self)
{
    // vetor com a nova capacidade 
    void *novo_espaco = malloc((self->cap / 2) * self->tam_dado);
    assert(novo_espaco != NULL);

    // movimenta a lista em ordem para o início do vetor
    for (int i = 0; i < self->n_elem; i++)
    {
        int pv = (self->pri + i) % self->cap;
        void *origem = calcula_ponteiro(self, pv);
        void *destino = (char*)novo_espaco + i * self->tam_dado;
        memmove(destino, origem, self->tam_dado);
    }
    // reduz a capacidade pela metade
    self->cap /= 2;
    // altera os índices de pri e fim
    self->pri = 0;
    self->fim = (self->pri + self->n_elem) % self->cap;

    // atualiza o ponteiro do vetor
    free(self->espaco);
    self->espaco = novo_espaco;
}



// remove o dado no início da fila e, se pdado não for NULL, copia o dado removido para *pdado
void fila_remove(Fila self, void *pdado)
{
    // verifica se a fila está vazia
    if (fila_vazia(self))
    {
        printf("FILA VAZIA, NÃO SE PODE REMOVER ITENS\n");
        return;
    }
    // verifica se tem muita memória sobrando
    if (self->n_elem - 1 < self->cap / 2 && self->cap / 2 >= 10)
    {
        realoca_vetor_menos(self);
    }

    // calcula um ponteiro para o índice do primeiro elemento da fila
    void *inicio_da_fila = calcula_ponteiro(self, self->pri);
    // se pdado não estiver NULL, copia o dado do início da fila para ele
    if (pdado != NULL)
    {
        memmove(pdado, inicio_da_fila, self->tam_dado);
    }

    // altera o índice do primeiro elemento
    self->pri = (self->pri + 1) % self->cap;
    // decrementa o número de elementos na fila
    self->n_elem--;
}


void fila_inicia_percurso(Fila self, int pos_inicial)
{
  // o percurso iniciará na posição fornecida
  self->pos_percurso = pos_inicial;
}

bool fila_proximo(Fila self, void *pdado)
{
    // inicia um percurso aos elementos da fila, a partir de uma posição
    // cada dado da fila, a partir dessa posição será acessado por chamadas a fila_proximo()
    // se a posição for positiva, o percurso vai desde essa posição, até o fim da fila
    // se a posição for negativa, o percurso vai desde essa posição, até o início
    //   0 é a posição do primeiro dado (aquele que está na fila há mais tempo)
    //   1 é a posição do segundo dado, etc
    //   além disso,
    //   -1 é a posição do último dado (o que está na fila há menos tempo)
    //   -2 é a posição do penúltimo dado, etc

    // posição do dado na lista
    int pos_na_lista;
    // se a posição for negativa, converte ela
    if (self->pos_percurso < 0)
    {
        pos_na_lista = self->pos_percurso + self->n_elem;
    }
    else
    {
        pos_na_lista = self->pos_percurso;
    }
    // calcula o endereço de onde está o próximo dado do percurso
    int pos_no_vetor = (self->pri + pos_na_lista) % self->cap;
    void *proximo_dado = calcula_ponteiro(self, pos_no_vetor);
    // caso não exista dado nessa posição, o percurso terminou
    if (proximo_dado == NULL) return false;
    // copia o dado na posição para pdado
    if (pdado != NULL) memmove(pdado, proximo_dado, self->tam_dado);
    // calcula a posição do próximo dado do percurso
    if (self->pos_percurso < 0)
    {
        self->pos_percurso--;
    }
    else
    {
        self->pos_percurso++;
    }
    // se percorreu todos os elementos, o percurso acabou
    if (self->pos_percurso > self->n_elem || self->pos_percurso < -self->n_elem - 1)
    {
        return false;
    }
    // o dado foi encontrado, o percurso ainda não terminou
    return true;
}


void fila_imprime(Fila self)
{
    printf("n_elem: %d\n", self->n_elem);
    printf("cap: %d\n", self->cap);
    printf("pri: %d\n", self->pri);
    printf("fim: %d\n", self->fim);
    printf("vetor: [");
    int *vet = (int*)self->espaco;
    for (int i = 0; i < self->cap; i++)
    {
        printf("%d, ", vet[i]);
    }
    printf("]\n");
    printf("fila:  [");
    for (int i = 0; i < self->n_elem; i++)
    {
        printf("%d, ", vet[(self->pri + i) % self->cap]);
    }
    printf("]\n");
}


void *retorna_fila(Fila self)
{
    return self->espaco;
}

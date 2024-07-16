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
    bool consultado;         // usado por grafo_proxima_aresta(...)
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
    int no_em_consulta;              // usado por grafo_proxima_aresta(...)
    No_grafo *listas_de_adjacencia;  // vetor de listas de adjacência
};
// typedef struct _grafo *Grafo; >>> grafo.h


// indica se está sendo realizada uma consulta de arestas que partem (0) ou arestas que chegam (1) à um nó.
// -1 = não tem consulta sendo realizada
int tipo_consulta = -1;


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


void grafo_destroi(Grafo self)
{
    // libera as listas de adjacência
    for (int i = 0; i < self->n_nos; i++)
    {
        No_grafo p = self->listas_de_adjacencia[i];
        while (p != NULL)
        {
            No_grafo temp = p->prox;
            free(p->valor_no);
            free(p->valor_aresta);
            free(p);
            p = temp;
        }
    }
    // libera o grafo
    free(self->listas_de_adjacencia);
    free(self);
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
    novo_no->consultado = false;

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


void grafo_remove_no(Grafo self, int no)
{
    // 0-0: essa função podia ser otimizada mas dessa forma da pra enxergar todas as operações de forma mais clara

    // verifica se o grafo está vazio
    if (self->n_nos == 0)
    {
        printf("GRAFO VAZIO, NÃO PODE REMOVER NÓS\n");
        return;
    }

    // remove todas as arestas que incidem sobre o nó a ser removido
    for (int i = 0; i < self->n_nos; i++)
    {
        No_grafo p = self->listas_de_adjacencia[i];
        No_grafo p_ant = NULL;
        while (p != NULL && p->no_id != no)
        {
            p_ant = p;
            p = p->prox;
        }
        // se achou o nó e ele não for o primeiro da lista, remove-o
        // (a remoção da própria lista de adjacências do nó a ser removido será tratada depois)
        if (p != NULL)
        {
            if (p_ant == NULL)
            {
                self->listas_de_adjacencia[i] = p->prox;
            }
            else
            {
                p_ant->prox = p->prox;
            }
            free(p->valor_no);
            free(p->valor_aresta);
            free(p);
        }
    }

    // corrige a indentificação dos nós
    // "a identificação dos nós remanescentes é alterada, como se esse nó nunca tivesse existido"
    for (int i = 0; i < self->n_nos; i++)
    {
        No_grafo p = self->listas_de_adjacencia[i];
        while (p != NULL)
        {
            if (p->no_id > no)
            {
                p->no_id--;
            }
            p = p->prox;
        }
    }

    // libera a lista de adjacência do nó a ser removido
    No_grafo p = self->listas_de_adjacencia[no];
    while (p != NULL)
    {
        No_grafo temp = p->prox;
        free(p->valor_no);
        free(p->valor_aresta);
        free(p);
        p = temp;
    }

    // ajusta a posição das listas de adjacência no vetor
    for (int i = no; i < self->n_nos - 1; i++)
    {
        self->listas_de_adjacencia[i] = self->listas_de_adjacencia[i + 1];
    }
    p = self->listas_de_adjacencia[self->n_nos - 1];

    // se necessário, realoca memória para o vetor de listas de adjacência
    self->n_nos--;
    if (self->n_nos > self->cap / 2 && self->cap / 2 >= 10)
    {
        self->cap /= 2;
        realoca_vetor(self);
    }
}


void grafo_altera_valor_no(Grafo self, int no, void *pdado)
{
    memmove(self->listas_de_adjacencia[no]->valor_no, pdado, self->tam_no);
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
    // verifica se os índices de origem e destino são válidos
    if (origem >= self->n_nos || destino >= self->n_nos)
    {
        printf("ÍNDICES DE ORIGEM E/OU DESTINO DE ARESTA INVÁLIDO(S)\n");
        return;
    }

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
        int cont = 0;  // cabeça da lista não conta como aresta para si mesma
        while ((p != NULL && p->no_id != destino) || cont == 0)
        {
            p_ant = p;
            p = p->prox;
            cont++;
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


// coloca em pdado (se não for NULL) o valor associado à aresta, se existir
// retorna true se a aresta entre os nós origem e destino existir, e false se não existir
bool grafo_valor_aresta(Grafo self, int origem, int destino, void *pdado)
{
    // percorre a lista de adjacência do nó em origem
    No_grafo p = self->listas_de_adjacencia[origem]->prox;
    while (p != NULL)
    {
        // se existir uma aresta até o nó destino
        if (p->no_id == destino)
        {
            memmove(pdado, p->valor_no, self->tam_no);
            return true;
        }
        p = p->prox;
    }
    return false;
}


void grafo_arestas_que_partem(Grafo self, int origem)
{
    // marca a realização de uma consulta de arestas que partem
    tipo_consulta = 0;
    self->no_em_consulta = origem;
}


void grafo_arestas_que_chegam(Grafo self, int destino)
{
    // marca a realização de uma consulta de arestas que chegam
    tipo_consulta = 1;
    self->no_em_consulta = destino;
}


// marca todas as arestas como não consultadas
static void marca_tudo_nao_consultado(Grafo self)
{
    for (int i = 0; i < self->n_nos; i++)
    {
        No_grafo p = self->listas_de_adjacencia[i];
        while (p != NULL)
        {
            p->consultado = false;
            p = p->prox;
        }
    }
}


// retorna a próxima aresta, de acordo com a última consulta iniciada por
//   'grafo_arestas_que_partem' ou 'grafo_arestas_que_chegam'
// o valor do nó vizinho ao nó da consulta deve ser colocado em 'vizinho' (se não for NULL),
//   o valor associado à aresta deve ser colocado em '*pdado' (se não for NULL) e a função
//   deve retornar true. Caso não exista mais aresta que satisfaça a consulta, retorna
//   false.
bool grafo_proxima_aresta(Grafo self, int *vizinho, void *pdado)
{
    // se a consulta sendo feita é de arestas que partem
    if (tipo_consulta == 0)
    {
        // percorre a lista de adjacência do nó em consulta
        No_grafo p = self->listas_de_adjacencia[self->no_em_consulta]->prox;
        while (p != NULL)
        {
            // se achar uma aresta que não foi consultada
            if (!p->consultado)
            {
                // copia o número do nó destino da aresta para vizinho
                memmove(vizinho, &p->no_id, sizeof(int));
                // copia o valor da aresta para pdado
                memmove(pdado, p->valor_aresta, self->tam_aresta);
                // marca a aresta como consultada
                p->consultado = true;
                return true;
            }
            p = p->prox;
        }
        // Caso não exista mais aresta que satisfaça a consulta
        marca_tudo_nao_consultado(self);
        return false;
    }
    // se a consulta sendo feita é de arestas que chegam
    else if (tipo_consulta == 1)
    {
        // percorre cada lista de adjacência
        for (int i = 0; i < self->n_nos; i++)
        {
            No_grafo p = self->listas_de_adjacencia[i]->prox;
            while (p != NULL)
            {
                // se achar uma aresta que incide no nó em consulta
                if (p->no_id == self->no_em_consulta && !p->consultado)
                {
                    // copia o número do nó origem da aresta para vizinho
                    memmove(vizinho, &self->listas_de_adjacencia[i]->no_id, sizeof(int));
                    // copia o valor da aresta para pdado
                    memmove(pdado, p->valor_aresta, self->tam_aresta);
                    // marca a aresta como consultada
                    p->consultado = true;
                    return true;
                }
                p = p->prox;
            }
        }
        // Caso não exista mais aresta que satisfaça a consulta
        marca_tudo_nao_consultado(self);
        return false;
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

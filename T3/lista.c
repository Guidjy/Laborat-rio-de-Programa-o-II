#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"


// estrutura de um nó da lista
struct _no
{
    void *dado;          // ponteiro genêrico para o dado do nó
    char *tipo;          // tipo do dado armazenado no nó
    struct _no *ant;     // ponteiro para o nó anterior
    struct _no *prox;    // ponteiro para o próximo nó
};
// define um tipo que é um ponteiro para a estrutura do nó
typedef struct _no *No;


// estrutura de um descritor da lista
struct _lista
{
    int n_elem;     // número de elementos na lista
    No pri;         // ponteiro para o primeiro elemento da lista
    No ult;         // ponteiro para o último elemento da lista
};


Lista lista_cria()
{
    // aloca memória para a etrutura da lista
    Lista nova = (Lista)malloc(sizeof(struct _lista));
    if (nova == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA\n");
        return NULL;
    }
    // inicializa os descritores da lista
    nova->n_elem = 0;
    nova->pri = NULL;
    nova->ult = NULL;
    return nova;
}


static No no_cria(void *dado, char *tipo)
{
    // aloca memória para o nó
    No novo = (No) malloc(sizeof(struct _no));
    if (novo == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA\n");
        return NULL;
    }
    // inicializa os descritores
    if (strcmp(tipo, "int") == 0)
    {
        novo->dado = malloc(sizeof(int));
        memmove(novo->dado, dado, sizeof(int));
    }
    else if (strcmp(tipo, "float") == 0)
    {
        novo->dado = malloc(sizeof(float));
        memmove(novo->dado, dado, sizeof(float));
    }
    else if (strcmp(tipo, "char") == 0)
    {
        novo->dado = malloc(sizeof(char));
        memmove(novo->dado, dado, sizeof(char));
    }
    else if (strcmp(tipo, "string") == 0)
    {
        novo->dado = malloc(strlen(dado));
        strcpy(novo->dado, dado);
    }
    else if (strcmp(tipo, "TAD") == 0)
    {
        novo->dado = dado;
    }
    novo->tipo = tipo;
    novo->ant = NULL;
    novo->prox = NULL;
    return novo;
}


void lista_insere(Lista self, void *dado, char *tipo)
{
    // aloca memória para um novo nó
    No novo = no_cria(dado, tipo);
    if (novo == NULL)
    {
        return;
    }
    // encadeia o novo nó
    if (lista_vazia(self))
    {
        self->pri = novo;
        self->ult = novo;
    }                       
    else
    {
        self->pri->ant = novo;
        novo->prox = self->pri;
        self->pri = novo;
    }
    self->n_elem++;
}


void *lista_remove(Lista self)
{
    // verifica se a lista está vazia
    if (lista_vazia(self))
    {
        printf("LISTA VAZIA, NÃO SE PODE REMOVER ELEMENTOS\n");
        return NULL;
    }
    // guarda um ponteiro para o nó a ser removido
    No removido = self->pri;
    // desencadeia o primeiro nó e o libera
    if (self->n_elem == 1)
    {
        self->pri = NULL;
        self->ult = NULL;
    }
    else
    {
        self->pri = self->pri->prox;
        self->pri->ant = NULL;
    }
    self->n_elem--;
    return removido->dado;
}


void lista_insere_pos(Lista self, void *dado, char *tipo, int pos)
{
    // apoio para índices negativos (ex: -1 é o final da lista)
    if (pos < 0)
    {
        pos += self->n_elem;
    }
    // verifica se o índice é válido
    if (pos > self->n_elem)
    {
        printf("ÍNDICE DE INSERSÃO INVÁLIDO\n");
        return;
    }
    // inserção no início
    if (pos == 0)
    {
        lista_insere(self, dado, tipo);
    }
    // inserção no final
    else if (pos == self->n_elem)
    {
        lista_append(self, dado, tipo);
    }
    // inserção no meio
    else
    {
        // aloca memória para um novo nó
        No novo = no_cria(dado, tipo);
        if (novo == NULL)
        {
            return;
        }
        No p = self->pri;
        // acha a posição de insersão
        for (int i = 0; i < pos; i++)
        {
            p = p->prox;
        }
        // encadeia o nó
        novo->prox = p;
        novo->ant = p->ant;
        p->ant->prox = novo;
        p->ant = novo;
        self->n_elem++;
    }
}


void *lista_remove_pos(Lista self, int pos)
{
    // apoio para índices negativos (ex: -1 é o final da lista)
    if (pos < 0)
    {
        pos += self->n_elem;
    }
    // verifica se o índice é válido
    if (pos >= self->n_elem)
    {
        printf("ÍNDICE DE REMOÇÃO INVÁLIDO\n");
        return NULL;
    }
    // remoção no início
    if (pos == 0)
    {
        return lista_remove(self);
    }
    // remoção no final
    else if (pos == self->n_elem - 1)
    {
        return lista_pop(self);
    }
    // remoção no meio
    else
    {
        // ponteiros para pecorrer a lista
        No p = self->pri;
        No p_ant = NULL;
        // acha a posição de remoção
        for (int i = 0; i < pos; i++)
        {
            p_ant = p;
            p = p->prox;
        }
        // guarda um ponteiro para o dado do elemento a ser removido
        void *removido = p->dado;
        p_ant->prox = p->prox;
        p->prox->ant = p_ant;
        self->n_elem--;
        return removido;
    }
}


void lista_enqueue(Lista self, void *dado, char *tipo)
{
    // aloca memória para um novo nó
    No novo = no_cria(dado, tipo);
    if (novo == NULL)
    {
        return;
    }
    // encadeia o novo nó
    if (lista_vazia(self))
    {
        self->pri = novo;
        self->ult = novo;
    }
    else
    {
        novo->ant = self->ult;
        self->ult->prox = novo;
        self->ult = novo;
    }
    self->n_elem++;
}


void *lista_dequeue(Lista self)
{
    return lista_remove(self);
}


void lista_preppend(Lista self, void *dado, char *tipo)
{
    lista_insere(self, dado, tipo);
}


void lista_append(Lista self, void *dado, char *tipo)
{
    lista_enqueue(self, dado, tipo);
}


void *lista_pop(Lista self)
{
    // verifica se a lista está vazia
    if (lista_vazia(self))
    {
        printf("LISTA VAZIA, NÃO SE PODE REMOVER ELEMENTOS\n");
        return NULL;
    }
    // guarda um ponteiro para o nó a ser removido
    No removido = self->ult;
    if (self->n_elem == 1)
    {
        self->pri = NULL;
        self->ult = NULL;
    }
    else
    {
        self->ult->ant->prox = NULL;
    }
    self->n_elem--;
    return removido->dado;
}


bool lista_vazia(Lista self)
{
    return self->n_elem == 0;
}


int lista_n_elem(Lista self)
{
    return self->n_elem;
}


void *lista_retorna_dado(Lista self, int pos)
{
    No p = self->pri;
    for(int i = 0; i < pos; i++)
    {
        p = p->prox;
    }
    if (p == NULL)
    {
        return NULL;
    }
    return p->dado;
}


// Função auxiliar da função sort --- GPT fez ↓↓↓
static float convert_to_float(void *data, char *tipo) {
    if (strcmp(tipo, "float") == 0) 
    {
        return *(float *)data;
    } 
    else if (strcmp(tipo, "int") == 0) 
    {
        return (float)(*(int *)data);
    } 
    else if (strcmp(tipo, "char") == 0) 
    {
        return (float)(*(char *)data);
    }
    else if (strcmp(tipo, "string") == 0) 
    {
        return (float)((unsigned char)(*(char *)data));
    }
    return 0.0f;
}

// Bubble Sort function to sort the list --- GPT fez ↓↓↓
void lista_sort(Lista self) {
    // se a lista estiver vazia
    if (self == NULL || self->pri == NULL) 
    {
        return;
    }
    // variável de controle que indica se houve uma troca de posição na lista
    int swapped;
    // ponteiros para percorrer a lista
    No ptr1;
    No lptr = NULL;

    // Bubble Sort
    do 
    {
        // reseta a variável de controle
        swapped = 0;
        // aponta para o início da lista
        ptr1 = self->pri;
        // percorre a lista
        while (ptr1->prox != lptr) 
        {
            // obtem os dados convertidos para float do nó apontado por prt1 e prt1->prox
            float data1 = convert_to_float(ptr1->dado, ptr1->tipo);
            float data2 = convert_to_float(ptr1->prox->dado, ptr1->prox->tipo);
            // se o valor do nó de prt1 for maior que o de prt1->prox
            if (data1 > data2) 
            {
                // troca eles de lugar
                void *temp_dado = ptr1->dado;
                char *temp_tipo = ptr1->tipo;

                ptr1->dado = ptr1->prox->dado;
                ptr1->tipo = ptr1->prox->tipo;

                ptr1->prox->dado = temp_dado;
                ptr1->prox->tipo = temp_tipo;

                // altera a variável de controle
                swapped = 1;
            }
            // analisa o próximo nó
            ptr1 = ptr1->prox;
        }
        lptr = ptr1;
    } while (swapped);
}


void lista_imprime(Lista self)
{
    if (lista_vazia(self))
    {
        printf("()\n");
        printf("n_elem = %d\n", self->n_elem);
        return;
    }
    printf("(");
    No p = self->pri;
    while (p->prox != NULL)
    {
        if (strcmp(p->tipo, "int") == 0)
        {
            printf("%d, ", *(int*)p->dado);
        }
        else if (strcmp(p->tipo, "float") == 0)
        {
            printf("%f, ", *(float*)p->dado);
        }
        else if (strcmp(p->tipo, "char") == 0)
        {
            printf("%c, ", *(char*)p->dado);
        }
        else if (strcmp(p->tipo, "string") == 0)
        {
            printf("\"%s\", ", (char*)p->dado);
        }
        p = p->prox;
    }
    if (strcmp(p->tipo, "int") == 0)
    {
        printf("%d)\n", *(int*)p->dado);
    }
    else if (strcmp(p->tipo, "float") == 0)
    {
        printf("%f)\n", *(float*)p->dado);
    }
    else if (strcmp(p->tipo, "char") == 0)
    {
        printf("%c)\n", *(char*)p->dado);
    }
    else if (strcmp(p->tipo, "string") == 0)
    {
        printf("\"%s\")\n", (char*)p->dado);
    }
    printf("n_elem = %d\n", self->n_elem);
}


void lista_libera(Lista self)
{
    // ponteiro para percorrer a lista
    No p = self->pri;
    // libera os nós
    while(p != NULL)
    {
        No temp = p->prox;
        free(p);
        p = temp;
    }
    // libera o descritor
    free(self);
}

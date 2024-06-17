// árvore binária de busca de strings para o T3 de Lab II
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "abb.h"


// estrutura de um nó da árvore
struct _no_arv
{
    int lin;                // posição vertical do nó na tela
    int col;                // posição horizontal do nó na tela
    int pos_no_nivel;       // posição da árvore no nível
    char *palavra;          // string com a palavra armazenada no nó
    struct _no_arv *esq;    // ponteiro para o filho esquerdo
    struct _no_arv *dir;    // ponteiro para o filho direito
};


// nós vazios são representados por um nó com ponteiros nulos
static arv cria_no_vazio()
{
    // aloca memória para a estrutura do nó
    arv novo = (arv) malloc(sizeof(struct _no_arv));
    if (novo == NULL)
    {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA\n");
        return NULL;
    }
    // deixa os ponteiros nulos
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}


arv arv_cria()
{
    return cria_no_vazio();
}


bool arv_vazia(arv self)
{
    return self->esq == NULL && self->dir == NULL;
}


// retorna true se o nó for uma folha
bool eh_folha(arv self)
{
    return arv_vazia(self->esq) && arv_vazia(self->dir);
}


// retorna true as duas strings passadas forem iguais
bool strings_iguais(char *a, char *b)
{
    return strcmp(a, b) == 0;
}


// retorna true se "a" preceder "b" alfabeticamente
bool string_menor(char *a, char *b)
{
    // strcmp retorna:
    // < 0 quando os valores ascii de "a" < "b"
    // 0 quando forem iguais
    // > 0 quando "a" > "b"
    return strcmp(a, b) < 0;
} 


void arv_insere(arv self, char *pal)
{
    // guarda a posição da árvore no nível (necessário para impressão)
    static int pos_no_nivel = 0;
    // se a árvore / sub-árvore estiver vazia, altera os dados da raíz
    if (arv_vazia(self))
    {
        self->palavra = (char *) malloc(strlen(pal) + 1);
        self->pos_no_nivel = pos_no_nivel;
        strcpy(self->palavra, pal);
        self->esq = cria_no_vazio();
        self->dir = cria_no_vazio();
        pos_no_nivel = 0;
    }
    // se a palavra que quer inserir ja estiver na árvore, não insere
    else if (strings_iguais(pal, self->palavra))
    {
        ;
    }
    // se a palavra que quer inserir preceder a palavra da árvore alfabeticamente
    // deve-se inseri-la na sub-árvore esquerda
    else if (string_menor(pal, self->palavra))
    {
        pos_no_nivel = pos_no_nivel * 2;
        arv_insere(self->esq, pal);
    }
    // se não, insere na sub-árvore direita
    else
    {
        pos_no_nivel = pos_no_nivel * 2 + 1; 
        arv_insere(self->dir, pal);
    }
}


////  funções auxiliares da função de remoção

// libera os nós filhos e torna o nó vazio
void esvazia_no(arv self)
{
    self->palavra = "\0";
    free(self->esq);
    free(self->dir);
    self->esq = self->dir = NULL;
}


// retorna a palavra com os maiores valores ascii da árvore
char *arv_maior_palavra(arv self)
{
    // desce até o nó mais da direita
    assert(!arv_vazia(self)); // esta função não deve ser usada em árvores vazias
    if (arv_vazia(self->dir))
    {
        return self->palavra;
    }
    return arv_maior_palavra(self->dir);
}


// retorna a palavra com os menores valores ascii da árvore
char *arv_menor_palavra(arv self)
{
    // desce até o nó mais da esquerda
    assert(!arv_vazia(self)); // esta função não deve ser usada em árvores vazias
    if (arv_vazia(self->esq))
    {
        return self->palavra;
    }
    arv_menor_palavra(self->esq);
}


bool arv_remove(arv self, char *pal)
{
    // variável auxiliar que indica se ouve remoção
    bool foi_removido = false;
    // se a árvore estiver vazia, não da pra remover
    if (arv_vazia(self)) return foi_removido;
    // se encontrar o valor a ser removido
    if (strings_iguais(pal, self->palavra))
    {
        // marca a remoção com sucesso
        foi_removido = true;
        // caso fácil: elemento a ser removido está numa folha
        if (eh_folha(self))
        {
            esvazia_no(self);
        }
        // caso menos fácil: nó tem filhos
        else
        {
            // nova palavra a colocar no nó
            char *nova_palavra;
            // preferência pela esquerda (poderia ser pela direita ou aleatório)
            if (!arv_vazia(self->esq))
            {
                // nova palavra recebe a maior palavra da sub-árvore esquerda
                nova_palavra = (char*) malloc(strlen(arv_maior_palavra(self->esq) + 1));
                strcpy(nova_palavra, arv_maior_palavra(self->esq));
                // chama a função novamente, removendo a nova palavra da sub-árvore esquerda
                arv_remove(self->esq, nova_palavra);
            }
            else
            {
                nova_palavra = (char*) malloc(strlen(arv_menor_palavra(self->dir) + 1));
                strcpy(nova_palavra, arv_menor_palavra(self->dir));
                arv_remove(self->dir, nova_palavra);
            }
            self->palavra = nova_palavra;
        }
    }
    // se a palavra for menor que a do nó, busca na esquerda
    else if (string_menor(pal, self->palavra))
    {
        arv_remove(self->esq, pal);
    }
    // se não, busca na direita
    else
    {
        arv_remove(self->dir, pal);
    }
}


// retorna o maior valor entre dois
static int maior(int a, int b)
{
    if (a > b) {
        return a;
    } else if (a < b) {
        return b;
    }
    return a;
}


int arv_altura(arv self)
{
    // se a árovre está vazia, sua altura é -1
    if (arv_vazia(self)) return -1;
    // altura da árvore = 1 + altura da árvore filha mais alta
    return 1 + maior(arv_altura(self->esq), arv_altura(self->dir));
}


int arv_fator_de_equilibrio(arv self)
{
    // o fator de equilíbrio geralmente é calculado como a altura do filho esquerdo menos a do direito
    return arv_altura(self->esq) - arv_altura(self->dir);
}


void arv_imprime(arv raiz, int nivel)
{
    if (arv_vazia(raiz)) return;
    // Imprime a subárvore direita
    arv_imprime(raiz->dir, nivel + 1);
    // Imprime o nó atual
    for (int i = 0; i < nivel; i++)
        printf("   ");
    printf("%s\n", raiz->palavra);
    // Imprime a subárvore esquerda
    arv_imprime(raiz->esq, nivel + 1);
}


void arv_libera(arv self)
{
    if (arv_vazia(self))
    {
        return;
    }
    arv_libera(self->esq);
    arv_libera(self->dir);
    esvazia_no(self);
}

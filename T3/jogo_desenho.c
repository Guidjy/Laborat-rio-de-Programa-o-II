#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "jogo_arv.h"
#include "tecla.h"
#include "tela.h"
#include "abb.h"
#include "lista.h"

#define LARGURA_MAX 134
#define ALTURA_MAX 37

// cada palavra tem um tempo até ser inserida.
struct _word
{
    char palavra[17];
    double tempo;
};


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


// por algum motivo, a função pow() de math.h estava dando erro
int power(int x, int y)
{
    int power = x;
    if (y == 0) return 1;
    for (int i = 1; i < y; i++)
    {
        power *= x;
    }
    return power;
}


// retorna o número de caracteres necessários para imprimir uma árvore
int calculo_dimensional(arv self)
{
    // se a árvore está vazia, ela não tem dimensões
    if (arv_vazia(self)) return 0;
    // calcula a largura da subárvore esquerda
    int largura_esquerda = calculo_dimensional(self->esq);
    // calcula a largura da subárvore direita
    int largura_direita = calculo_dimensional(self->dir);
    
    int largura = (int)((strlen(self->palavra) + largura_esquerda + largura_direita) * 1.2);
    if (largura > LARGURA_MAX - 21)
    {
        return LARGURA_MAX - 21;
    }
    return largura;
}


void desenha_arvore(arv self, int largura)
{
    //// um percurso em largura que imprime todos os nós

    // verifica se a árvore está vazia
    if (arv_vazia(self)) return;

    // cria uma fila para armazenar árvores
    Lista fila_de_arvores = lista_cria();
    // insere o nó raíz da árvore ou sub-árvore
    lista_enqueue(fila_de_arvores, self, "TAD");

    // espaço horizontal disponível para a impressão de uma palavra
    int espaco_x;
    // guarda o nivel atual da árvore
    int nivel = 0;

    // enquanto a fila não estiver vazia
    while (!lista_vazia(fila_de_arvores)) 
    {
        // guarda o número de nós no nível atual
        int n_nos_nivel = lista_n_elem(fila_de_arvores);
        // atualiza o espaço disponível para cada palavra
        espaco_x = largura / power(2, nivel);

        // guarda o nó atual
        arv no_atual;
        
        // percorre todos os nós do nível atual
        while (n_nos_nivel > 0) 
        {
            // desenfilera a fila e guarda o nó desenfilerado para impressão
            no_atual = (arv)lista_dequeue(fila_de_arvores);

            // calcula a posição vertical do nó
            no_atual->lin = (nivel + 1) * 2;
            // calcula a posição horizontal do nó
            no_atual->col = espaco_x * no_atual->pos_no_nivel;
            no_atual->col += (espaco_x / 2) - (strlen(no_atual->palavra) / 2);
            // imprime a palavra no nó atual
            tela_lincol(no_atual->lin, no_atual->col);
            printf("%s", no_atual->palavra);
            // imprime setas para os filhos
            if (!eh_folha(no_atual))
            {
                tela_lincol(no_atual->lin + 1, no_atual->col - 1);
                printf("/");
                for (int i = 0; i < strlen(no_atual->palavra); i++)
                {
                    printf(" ");
                }
                printf("\\");
            }

            // enfileira os filhos do nó atual
            if (!arv_vazia(no_atual->esq)) lista_enqueue(fila_de_arvores, no_atual->esq, "TAD");
            if (!arv_vazia(no_atual->dir)) lista_enqueue(fila_de_arvores, no_atual->dir, "TAD");

            // decrementa o número de nós no nível atual (desenfilerou um nó para impressão)
            n_nos_nivel--;
        }
        // desce um nível
        nivel++;
    }
}


void desenha_duracao(double tempo)
{
    tela_lincol(2, LARGURA_MAX - 20);
    printf("| duração: ");
    tela_cor_letra(0, 255, 255);
    printf("%.2lf", tempo);
    tela_cor_normal();
}


void desenha_palavra_a_inserir(char *palavra, double tempo)
{
    int pos_y = tela_nlin() * tempo;
    pos_y += 4;
    tela_lincol(pos_y, LARGURA_MAX - 16);
    printf("%s", palavra);
}


void desenha_borda(char *entrada, int pontuacao)
{
    // borda da palavra a ser inserida
    for (int i = 0; i < tela_nlin() + 5; i++)
    {
        tela_lincol(i, LARGURA_MAX - 20);
        if (i == 3)
        {
            printf("├");
        }
        else
        {
            printf("│");
        }
        tela_lincol(i, LARGURA_MAX - 1);
        if (i == 3)
        {
            printf("┤");
        }
        else
        {
            printf("│");
        }
    }
    for (int i = LARGURA_MAX - 19; i < LARGURA_MAX - 1; i++)
    {
        tela_lincol(3, i);
        printf("─");
    }

    // borda da tela
    for (int i = 0; i < LARGURA_MAX; i++)
    {
        tela_lincol(tela_nlin() + 4, i);
        if (i == LARGURA_MAX - 20 || i == LARGURA_MAX - 1)
        {
            printf("┴");
        }
        else
        {
            printf("─");
        }
        tela_lincol(0, i);
        if (i == LARGURA_MAX - 20 || i == LARGURA_MAX - 1)
        {
            printf("┬");
            continue;
        }
        else
        {
            printf("─");
        }
    }

    // mostra a entrada do jogador
    tela_lincol(tela_nlin() + 6, 0);
    printf(">Entrada: ");
    tela_cor_letra(0, 255, 255);
    printf("%s", entrada);
    tela_cor_normal();
    tela_lincol(tela_nlin() + 7, 0);
    printf(">Pontos:  ");
    tela_cor_letra(0, 255, 255);
    printf("%d", pontuacao);
    tela_cor_normal();
}


void desenha_fim_de_jogo(int pontuacao, double duracao)
{
    tela_lincol(11, 50);
    tela_cor_letra(255, 51, 51);
    printf("GAME OVER!");
    tela_cor_normal();
    tela_lincol(13, 44);
    printf(">Pontuação:       %d", pontuacao);
    tela_lincol(14, 44);
    printf(">Tempo de jogo:   %.2lf", duracao);
    tela_lincol(16, 44);
    printf("Quer jogar denovo[s/n]? \n");
}

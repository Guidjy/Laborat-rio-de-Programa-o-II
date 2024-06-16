// Funções auxiliares do T3 de LabII

#ifndef _JOGO_ARV_H_
#define _JOGO_ARV_H_

#include "abb.h"

// TAD que guarda uma palavra e seu tempo de inserção
typedef struct _word *Palavra;

// verifica se o jogo terminou
bool game_over(arv a, int resultado);

// processa a entrada do jogador
int processa_entrada(char *palavra, char letra);

// preenche a lista de silabas
void preenche_lista_de_silabas(char silabas[104][5]);

// gera uma palavra aleatória e sorteia um tempo até sua inserção
Palavra gera_palavra_aleatoria(char silabas[104][5], double t_min, double t_max);

// diminui os limites de tempo de inserção de palavras
void diminui_limites_de_tempo(double duracao, double *t_max, double *t_min);

// atualiza o arquivo de maiores recordistas
void atualiza_top5(int pontuacao);

// retorna o número de caracteres necessários para imprimir uma árvore
int calculo_dimensional(arv self);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "tecla.h"
#include "tela.h"
#include "abb.h"
#include "jogo_arv.h"


struct _word
{
    char palavra[17];   // palavra
    double tempo;       // tempo até a inserção da palavra na árvore
};
// typedef _word *Palavra >>> jogo_arv.h


int main()
{
    // inicia o gerador de números aleatórios
    srand(time(0));
    // inicia o processamento de entrada de letras individuais
    tecla_ini();

    // lista de sílabas para gerar palavras
    char silabas[104][5];
    preenche_lista_de_silabas(silabas);

    // guarda uma palavra gerada aleatoriamente
    Palavra sorteada = NULL;
    // árvore com as palavras geradas
    arv arvore_de_palavras = arv_cria();

    // string com a palavra sendo digitada pelo jogador
    char entrada[17] = "\0";
    // buffer para receber a entrada do jogador
    char letra;
    // variável que guarda o resultado do processamento da entrada do jogador
    int resultado;

    // limites mínimo e máximo de tempo (em segundos) até a inserção de uma palavra na árvore
    double tempo_min = 4.0;
    double tempo_max = 8.0;

    // guarda o instante em que o jogo começou
    double tempo_inicio_jogo = tela_relogio();
    // guarda a duração do jogo
    double tempo_duracao_jogo;
    // guarda o instante em que uma palavra foi sorteada
    double tempo_sorteacao_palavra;
    // guarda o tempo até a inserção de uma palavra
    double tempo_ate_insercao;

    // variável de controle
    bool tem_palavra_sorteada = false;

    // pontuação
    int pontuacao = 0;

    while (true)
    {
        // laço principal do jogo
        do
        {
            //// 1) o processamento de teclas da entrada
            // le uma entrada do jogador
            letra = tecla_le_char();
            // processa a entrada (retorna 0, 1 ou 2 >>> ver função em jogo_arv.c)
            resultado = processa_entrada(entrada, letra);

            //// 2) a remoção de palavra digitada da árvore
            if (resultado == 1)
            {
                // remove a palavra digitada da árvore
                bool foi_removido = arv_remove(arvore_de_palavras, entrada);
                if (foi_removido)
                {
                    // aumenta a pontuação
                    pontuacao += 10;
                }
                // limpa a entrada
                while(strlen(entrada) > 0)
                {
                    entrada[strlen(entrada) - 1] = '\0';
                }
            }
            
            //// 3) geração da palavra aleatória
            // se não tiver uma palavra sorteada, sorteia uma nova
            if (!tem_palavra_sorteada)
            {
                // sorteia palavra
                sorteada = gera_palavra_aleatoria(silabas, tempo_min, tempo_max);
                // guarda o instante em que a palavra foi sorteada
                tempo_sorteacao_palavra = tempo_duracao_jogo;
                // guarda o tempo até a inserção da palavra
                tempo_ate_insercao = sorteada->tempo;
                // marca a existência de uma palavra sorteada
                tem_palavra_sorteada = true;
            }
            // se ja tiver uma palavra sorteada
            else
            {
                // diminui seu tempo até inserção
                tempo_ate_insercao = sorteada->tempo - (tempo_duracao_jogo - tempo_sorteacao_palavra); 
                // se o tempo até a inserção esgotar:
                if (tempo_ate_insercao <= 0)
                {
                    //// 4) inserção da palavra aleatória na árvore
                    // insere a palavra na árvore
                    arv_insere(arvore_de_palavras, sorteada->palavra);
                    // marca a inexistência de uma palavra sorteada
                    tem_palavra_sorteada = false;
                }
            }

            // atualiza o tempo de duração do jogo
            tempo_duracao_jogo = tela_relogio() - tempo_inicio_jogo;
            // diminui os limites de tempo de inserção de palavras
            diminui_limites_de_tempo(tempo_duracao_jogo, &tempo_max, &tempo_min);

            // 5) desenha a árvore
            printf("\n\n");
            arv_imprime(arvore_de_palavras, 0);
            printf("\n\n");
            printf("palavra sorteada: %s \n", sorteada->palavra);
            printf("tempo até inserção: %.2lf\n", tempo_ate_insercao);
            printf("duração: %.2lf\n", tempo_duracao_jogo);
            printf("min: %.2lf\nmax: %.2lf\n", tempo_min, tempo_max);
            printf("resultado: %d\n", resultado);
            printf("entrada: %s\n", entrada);
        }
        while (!game_over(arvore_de_palavras, resultado));

        printf("GAME OVER!\n");
        // se uma partida encerrar, pergunta se o jogador quer continuar jogando
        printf("Quer jogar denovo[s/n]? \n");
        char resposta;
        do 
        {
            resposta = tolower(tecla_le_char());
        }
        while (resposta != 's' && resposta != 'n');
        if (resposta == 'n') 
        {
           break;
        }
    }

    // encerra o processamento de letras individuais
    tecla_fim();

    // TODO tela de final
    printf("Pontuação: %d\n", pontuacao);
    
    return 0;
}

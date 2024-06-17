#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "tecla.h"
#include "tela.h"
#include "lista.h"
#include "abb.h"
#include "jogo_arv.h"
#include "jogo_desenho.h"


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
    // inicia a tela especial
    tela_ini();

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
    double tempo_min = 3.5;
    double tempo_max = 7.0;

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
                    pontuacao += strlen(entrada);
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

            // 5) desenha a árvore e a tela
            tela_limpa();

            int largura = calculo_dimensional(arvore_de_palavras);
            desenha_arvore(arvore_de_palavras, largura);

            desenha_duracao(tempo_duracao_jogo);
            desenha_palavra_a_inserir(sorteada->palavra, tempo_ate_insercao / sorteada->tempo);
            desenha_borda(entrada, pontuacao);

            tela_atualiza();
        }
        while (!game_over(arvore_de_palavras, resultado));
        
        tela_limpa();
        // se uma partida encerrar, pergunta se o jogador quer continuar jogando
        desenha_fim_de_jogo(pontuacao, tempo_duracao_jogo);
        tela_atualiza();
        // recebe a resposta do jogador
        char resposta;
        do 
        {
            resposta = tolower(tecla_le_char());
        }
        while (resposta != 's' && resposta != 'n');
        // se não quiser jogar denovo, quebra o laço
        if (resposta == 'n') 
        {
           break;
        }
        // se quiser jogar denovo
        if (resposta == 's')
        {
            // reinicia as variáveis do jogo
            tempo_min = 3.5;
            tempo_max = 7.0;
            tempo_inicio_jogo = tela_relogio();
            pontuacao = 0;
            arv_libera(arvore_de_palavras);
            while(strlen(entrada) > 0)
            {
                entrada[strlen(entrada) - 1] = '\0';
            }
            // sorteiam-se duas palavras aqui para evitar o sorteamento de lixo ao reiniciar a partida
            sorteada = gera_palavra_aleatoria(silabas, tempo_min, tempo_max);
            sorteada = gera_palavra_aleatoria(silabas, tempo_min, tempo_max);
            tem_palavra_sorteada = false;
        }
    }

    // atualiza o arquivo dos maiores recordistas
    tela_limpa();
    atualiza_top5(pontuacao);

    tecla_fim();
    tela_fim();

    return 0;
}

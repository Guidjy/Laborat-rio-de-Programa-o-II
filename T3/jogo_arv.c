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


bool game_over(arv a, int resultado)
{
    // jogo termina se:
    // a) a árvore estiver desequilibrada
    if (!arv_vazia(a))
    {
        if (arv_fator_de_equilibrio(a) <= -2 || arv_fator_de_equilibrio(a) >= 2)
        {
            return true;
        }
    }
    // b) o jogador cancelar o jogo
    if (resultado == 2)
    {
        return true;
    }
    return false;
}


int processa_entrada(char *palavra, char letra)
{
    /*
    Deve ser mantida uma string com a palavra sendo digitada (palavra^^^). 
    O processamento da entrada lê uma tecla do teclado, e reage da seguinte forma, 
    dependendo do valor da tecla:
       uma letra - insere a letra no final da palavra.
       backspace - remove a última letra da palavra, se houver.
       enter     - marca a palavra como completa para remoção.
       escape    - marca o cancelamento do programa, após pedir confirmação.

    A função tem três valores de retorno possíveis:
       0 - palavra não está pronta para remoção
       1 - palavra está pronta para remoção
       2 - jogador pediu o cancelamento do programa
    */

    // se o caractere digitado for uma letra
    if (isalpha(letra))
    {
        // insere a letra no final da palavra
        int i = 0;
        while (palavra[i] != '\0')
        {
            i++;
        }
        palavra[i] = letra;
        return 0;
    }
    // se o caractere digitado for "Backspace"
    else if (letra == '\b')
    {
        // remove a letra no final da palavra
        palavra[strlen(palavra) - 1] = '\0';
        return 0;
    }
    // se o caractere digitado for "Enter"
    else if (letra == '\n')
    {
        // marca a palavra como completa para remoção
        return 1;
    }
    // se o caractere digitado for "ESC"
    else if (letra == '\033')
    {
        tela_limpa();
        tela_lincol(12, 40);
        // pede confirmação do jogador para cancelar o jogo
        printf("Tem certeza que quer cancelar o jogo[s/n]? \n");
        tela_atualiza();
        char confirmacao;
        do 
        {
            confirmacao = tolower(tecla_le_char());
        }
        while (confirmacao != 's' && confirmacao != 'n');
        if (confirmacao == 's') 
        {
           return 2; 
        }
    }
    return 0;
}


void preenche_lista_de_silabas(char silabas[104][5])
{
    int n_silabas = 0;
    char vogais[6] = "aeiou"; 
    char silaba[5];
    // passa pelo alfabeto
    for (int i = 0; i < 26; i++)
    {
        silaba[0] = 'a' + i;
        // primeira letra deve ser uma consoante
        // se não for, passa para a próxima letra do alfabeto
        if (silaba[0] == 'a' ||
            silaba[0] == 'e' ||
            silaba[0] == 'i' ||
            silaba[0] == 'o' ||
            silaba[0] == 'u')
        {
            continue;
        }
        // cria as diferentes silabas possíveis
        // (ex: silaba[0] = 's' >>> "sa", "se", "si"...)
        for (int j = 0; j < 5; j++)
        {
            silaba[1] = vogais[j];
            silaba[2] = '\0';
            // lista de silabas recebe a silaba
            strcpy(silabas[n_silabas], silaba);
            n_silabas++;
        }
    }
}


Palavra gera_palavra_aleatoria(char silabas[104][5], double t_min, double t_max)
{
    // palavra a ser retornada
    Palavra nova_palavra = (Palavra) malloc(sizeof(struct _word));
    assert(nova_palavra != NULL);

    // sorteia o número de sílabas (entre 1 e 5)
    int n_silabas = (rand() % 5) + 1;
    // sorteia n_silabas silabas
    for (int i = 0; i < n_silabas; i++)
    {
        int pos = rand() % 104;
        strcat(nova_palavra->palavra, silabas[pos]);
    }
    strcat(nova_palavra->palavra, "\0");

    // sorteia um tempo até a inserção
    nova_palavra->tempo = (double)((rand() % (int)t_max) + (int)t_min);

    return nova_palavra;
}


void diminui_limites_de_tempo(double duracao, double *t_max, double *t_min)
{    
    *t_max -= 0.0002 * duracao;
    if (*t_max < 2.0)
    {
        *t_max = 3.0;
    }
    *t_min -= 0.0001 * duracao;
    if (*t_min < 1.0)
    {
        *t_min = 1.5;
    }
}


static void get_string(char s[20])
{
    char string[20];
    char caractere;
    int i = 0;
    while (true)
    {
        caractere = tecla_le_char();
        if (caractere == '\n' || i == 18)
        {
            break;
        }
        else if (!isalpha(caractere))
        {
            continue;
        }
        else
        {
            string[i] = caractere;
            i++;
            printf("%c", caractere);
        }
        tela_atualiza();
    }
    string[i] = ',';
    string[i+1] = '\0';
    strcpy(s, string);
}


void atualiza_top5(int pontuacao)
{
    // Arquivo com as 5 maiores pontuações
    FILE *top5;
    // abre o arquivo para leitura
    top5 = fopen("top5.csv", "r");
    if (top5 == NULL)
    {
        printf("ERRO NA ABERTURA DO ARQUIVO\n");
        return;
    }

    // vetores para armazenar os dados do arquivo
    char top5_nomes[6][20];   // armazena os nomes dos jogadores
    int top5_pontos[6];       // armazena os pontos dos jogadores

    int n_jogadores = 0;      // guarda o número de jogadores no arquivo
    char nome_jogador[20];    // guarda o nome do jogador dessa partida

    // buffers para ler os conteúdos do arquivo
    char buffer_nome[20];
    int buffer_pontuacao;
    // lê os dados do arquivo
    while (fscanf(top5, "%s %d", buffer_nome, &buffer_pontuacao) != EOF)
    {
        // insere-os nos vetores
        strcpy(top5_nomes[n_jogadores], buffer_nome);
        top5_pontos[n_jogadores] = buffer_pontuacao;
        n_jogadores++; 
    }
    fclose(top5);

    // imprime os dados lidos para testagem
    /*
    for (int i = 0; i < n_jogadores; i++)
    {
        tela_lincol(10 + i, 1);
        printf("%s, %d\n", top5_nomes[i], top5_pontos[i]);
    }
    tela_atualiza();
    */

    // se o top 5 não estiver cheio
    if (n_jogadores < 5)
    {
        // pede o nome do jogador atual e registra-o no top 5
        tela_lincol(13, 44);
        tela_cor_letra(255, 255, 102);
        printf("Parabéns! Você está entre os 5 melhores!");
        tela_lincol(14, 44);
        tela_cor_normal();
        printf("Digite seu nome: ");
        tela_atualiza();
        get_string(nome_jogador);
    }
    // se o top 5 ja tiver 5 membros
    else
    {
        // variável de controle que indica se o nome do jogador atual ja foi registrado
        bool registrado = false;
        // percorre o vetor de pontuações
        for (int i = 0; i < 5; i++)
        {
            // se a pontuação do jogador atual for maior do que a de alguem no top 5
            if (pontuacao > top5_pontos[i] && !registrado)
            {
                // pede o nome do jogador atual e registra-o no top 5
                tela_lincol(13, 44);
                tela_cor_letra(255, 255, 102);
                printf("Parabéns! Você está entre os 5 melhores!");
                tela_lincol(14, 44);
                tela_cor_normal();
                printf("Digite seu nome: ");
                tela_atualiza();
                get_string(nome_jogador);
                registrado = true;
            }
        }
    }
    
    // abre o arquivo para escrever
    top5 = fopen("top5.csv", "w");
    if (top5 == NULL)
    {
        printf("ERRO NA ABERTURA DO ARQUIVO\n");
        return;
    }

    // variável de controle que indica se o nome do jogador atual ja foi registrado
    bool registrado = false;
    // indica quantos jogadores/pontuações ja foram registradas no arquivo
    int n_registrados = 0;
    // percorre o vetor de jogadores e o vetor de pontuações
    for (int i = 0; n_registrados < n_jogadores + 1 && n_registrados < 5; i++)
    {
        // se a pontuação do jogador atual for maior que a de um jogador no top5
        if ((pontuacao > top5_pontos[i] && !registrado) || n_jogadores == 0)
        {
            // insere o jogador atual e sua pontuaçã no top5 
            fprintf(top5, "%s %d\n", nome_jogador, pontuacao);
            n_registrados++;
            registrado = true;
            // verifica se ja não foram registrados 5 jogadores no arquivo
            if (n_registrados == 5)
            {
                break;
            }
        }
        // testa se o número de jogadores é maior do que zero para não escrever lixo no arquivo
        if (n_jogadores > 0)
        {
            // escreve os dados do top5 no arquivo
            fprintf(top5, "%s %d\n", top5_nomes[i], top5_pontos[i]);
            n_registrados++;
        }
    }
}


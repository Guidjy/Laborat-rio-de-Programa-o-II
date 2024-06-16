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
        if (caractere == '\n' || i == 19)
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
    }
    string[i] = '\0';
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
    // listas para armazenar os dados do arquivo
    Lista top5_nomes = lista_cria();   // armazena os nomes dos jogadores
    Lista top5_pontos = lista_cria();  // armazena os pontos dos jogadores
    char buffer_nome[20];
    int buffer_pontuacao;
    // lê os dados do arquivo
    while (fscanf(top5, "%s %d", buffer_nome, &buffer_pontuacao) != EOF)
    {
        // insere-os nas listas
        lista_enqueue(top5_nomes, buffer_nome, "string");
        lista_enqueue(top5_pontos, &buffer_pontuacao, "int");
    }
    fclose(top5);
    // abre o arquivo para escrever
    top5 = fopen("top5.csv", "w");
    if (top5 == NULL)
    {
        printf("ERRO NA ABERTURA DO ARQUIVO\n");
        return;
    }
    // atualiza o top 5
    int n_jogadores = lista_n_elem(top5_nomes);
    // se o arquivo estiver vazio
    if (n_jogadores == 0)
    {
        // pede o nome do jogador atual
        char nome_jogador[20];
        printf("Parabéns! Você está entre os 5 melhores!\n");
        printf("Digite seu nome: \n");
        tela_atualiza();
        get_string(nome_jogador);
        // escreve seu nome e pontuação no arquivo
        fprintf(top5, "%s, %d\n", nome_jogador, pontuacao);
    }
    // mudar TODO
    for (int i = 0; i < n_jogadores; i++)
    {
        strcpy(buffer_nome, (char*)lista_dequeue(top5_nomes));
        buffer_pontuacao = *(int*)lista_dequeue(top5_pontos);
        // verifica se a pontuação dessa partida foi maior do que uma do top 10
        if (pontuacao > buffer_pontuacao)
        {
            // se sim, pede o nome do jogador atual
            char nome_jogador[20];
            printf("Parabéns! Você está entre os 5 melhores!\n");
            printf("Digite seu nome: \n");
            tela_atualiza();
            get_string(nome_jogador);
            // escreve seu nome e pontuação no arquivo
            fprintf(top5, "%s, %d\n", nome_jogador, pontuacao);
            // zera a pontuação para que ela não seja inserida denovo
            pontuacao = 0;
            i++;
        }
        // verifica novamente se ja não foram escritos 5 nomes
        if (i < n_jogadores)
        {
            fprintf(top5, "%s %d\n", buffer_nome, buffer_pontuacao);
        }
    }
    // fecha o arquivo e salva os dados
    fclose(top5);
    // libera as listas
    lista_libera(top5_nomes);
    lista_libera(top5_pontos);
}

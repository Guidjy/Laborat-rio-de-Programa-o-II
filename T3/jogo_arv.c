#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
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
        // pede confirmação do jogador para cancelar o jogo
        printf("Tem certeza que quer cancelar o jogo[s/n]? \n");
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
            // trata silabas que começam com "Q" (qua, que, qui, quo)
            if (silaba[0] == 'q')
            {
                if (vogais[j] == 'u') continue;
                silaba[1] = 'u';
                silaba[2] = vogais[j];
            }
            // trata o restante das sílabas
            else
            {
                silaba[1] = vogais[j];
                silaba[2] = '\0';
            }
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

    // sorteia o número de sílabas (entre 1 e 5)
    int n_silabas = (rand() % 5) + 1;
    // sorteia n_silabas silabas
    for (int i = 0; i < n_silabas; i++)
    {
        int pos = rand() % 104;
        strcat(nova_palavra->palavra, silabas[pos]);
    }

    // sorteia um tempo até a inserção
    nova_palavra->tempo = (double)((rand() % (int)t_max) + (int)t_min);

    return nova_palavra;
}


void diminui_limites_de_tempo(double duracao, double *t_max, double *t_min)
{    
    *t_max -= 0.0002 * duracao;
    if (*t_max < 4.0)
    {
        *t_max = 3.0;
    }
    *t_min -= 0.0001 * duracao;
    if (*t_min < 2.0)
    {
        *t_min = 1.0;
    }
}


// retorna a dimensão (espaço) horizontal necessária para o desenho da árvore iniciada nesse nó
int calculo_dimensional(arv a, int nivel, int *pos_x)
{
    // a     : nó de uma árvore
    // nivel : nível do nó 
    // pos_x : a posição horizontal onde o desenho dessa árvore deve iniciar

    // Um nó vazio não atualiza nada e ocupa espaço zero
    if (arv_vazia(a))
    {
        return pos_x;
    }
    // um nó não vazio ocupa o espaço de seus filhos mais o seu espaço
    else
    {
        return strlen(a->palavra) + calculo_dimensional(a->esq, nivel + 1, pos_x) + calculo_dimensional(a->dir, nivel + 1, pos_x);
    }
}

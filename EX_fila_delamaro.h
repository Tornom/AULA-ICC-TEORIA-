#ifndef HEADER_H_FILE
#define HEADER_H_FILE

#include <stdio.h>
#include <stdlib.h>

#define true  1
#define false 0

typedef struct s_cliente            //Struct para o cliente
{
    double tempo_chegada;              //Possui o tempo em que ele chegará na fila
    double tempo_atendimento;          //Possui o tempo em que ele será atendido pelo caixa
    struct s_cliente* proximo;      //E indica quem é o proximo cliente atrás dele

}CLIENTE;

typedef struct s_fila               //Struct para a fila
{
    int tamanho;                    //Guarda seu tamanho
    CLIENTE* primeiro;              //Quem é o primeiro cliente da fila
    CLIENTE* ultimo;                //Quem é o ultimo cliente da fila
}FILA;

FILA* cria_fila();         //Função para alocar memória para guardar uma fila

CLIENTE* cria_cliente(double tempo_atendimento,double tempo_chegada); //Aloca memoria para um ponteiro guardar a informação de um cliente

int enfileirar_cliente(FILA* fila,CLIENTE* cliente);           //Posiciona o cliente no final da fila

CLIENTE* desenfileirar_cliente(FILA* fila);                     //Remove o primeiro cliente da fila, retornando ela para quem o chamou

int inicializacao(int N_caixa,int N_pessoas,int N_fila);

int simulacao(FILA** vetor_fila,int N_pessoas, int N_caixa,int N_fila);

double calcula_tempo(FILA* vetor_fila, double tempo_atendimento, double tamnaho_fila);

int procura_menor_fila(FILA** vetor_fila,int N_fila);

int caixa(int qual_caixa,CLIENTE* client,double tempo_decorrido,CLIENTE* vetor_caixa[]);


#endif

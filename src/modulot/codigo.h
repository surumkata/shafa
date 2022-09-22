#ifndef CODIGO_H_INCLUDED
#define CODIGO_H_INCLUDED
#include "listas.h"


/**
  * brief Calcula o somatorio dos elementos de indice i a j do arreio freq
  * @param Arreio
  * @param Indice menor
  * @param Indice maior
  * @returns Somatorio
  */
int soma(int freq[], int i, int j);


/**
  * brief Calcula a melhor divisao de uma sequencia qualquer de freqencias
  * ordenadas entre o elemento na posicao i e o elemento na posição j
  * @param Arreio
  * @param Indice menor
  * @param Indice maior
  * @returns Indice da melhor divisao
  */
int getBestDivision(int freq[], int i, int j);


/**
  * brief Coloca a string de cada codigo a nulo
  * @param Arreio de strings
  * @param Tamanho do arreio
  */
void inicializeCodes(char codes[][256], int num_simb);


/**
  * brief Adiciona o caracter 'b'(0 ou 1) a todas as strings que se encontram entre os indices i e j de codes
  * @param Caracter '0' ou '1'
  * @param Arreio de strings
  * @param Indice menor
  * @param Indice maior
  */
void addBitCode(char b, char codes[][256], int i, int j);


/**
  * brief Calcula os codigos SF dos simbolos cujas frequencias se encontram no arreio freq entre as posicoes
  * i e j e guarda-os no arreio de strings codes
  * @param Arreio de frequencias
  * @param Arreio de strings
  * @param Indice menor
  * @param Indice maior
  * @param Tamanho arreio freq
  */
void getSFCodes(int freq[], char codes[][256], int i, int j, int s);


/**
  * brief Print dos tamanhos
  * @param Arreio com o tamanho de cada bloco analisado
  * @param Numero de blocos analisados
  */
void imprimeTamahos(int tam_blocos[], int num_blocos);


/**
  * brief Preenche o campo codigoSF de cada elemento da lista com o respetivo codigo que se encontra no arreio codes
  * @param Lista com os simbolos e respetivas frequencias armazenadas
  * @param Arreio de codigos(no formato string)
  */
void storeSFCodes(LISTA l, char codes[][256]);

#endif

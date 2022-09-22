/*
    Gonçalo Pereira & José Gomes
    funções utilitárias e de descodificação/descompressão
*/

#ifndef ___logica_h___
#define ___logica_h___
#include <stdio.h>
#include <stdlib.h>

/*
    Estrutura printMessage
    - Ficheiros produzidos pelos modulos
    - Tempo de execução
    - Número de Blocos
    - Tamanho dos Blocos antes de descompressão
    - Tamanho dos Blocos depois de descompressão
    Guarda toda a informação necessária sobre a execução do processo para impressão final.
*/

typedef struct printMessage {
    char* ficheiroRLE;
    char* ficheiroSHAF;
    int timer;
    int nBlocos;
    int* tamRLE;
    int* tamSHAF;
    int* tamDescompRLE;
    int* tamDescompSHAF;
    
}pMessage;

extern pMessage mensagem;

/*
    Estrutura codeArray
    - Simbolo que descodifica um código
    - Tamanho do Código
    Elemento do array que contém no indice[código] a sua descodificação e o tamanho do código para verificação. 
*/

typedef struct codeArray {
    int simbolo;
    int tamanho;
}cArray;

/*
    Estrutura argDescompBloco
    - Nome do ficheiro
    - Offset, onde começa a escrever no ficheiro final
    - Bloco, string de caracteres a descomprimir
    - Tamanho do bloco
    Guarda toda a informação necessária para a descompressão de um bloco por um Thread.
*/

typedef struct argDescompBloco {

    char* filename;
    int offset;
    char* bloco;
    int tamanho;

}argDB;

/*
    Estrutura argLeBloco
    - Nome do ficheiro
    - Offset, onde começa a ler no ficheiro
    - Bloco, local onde guarda a string de caracteres que lê
    - Tamanho do bloco a ler
    - Tamanho do bloco depois de descomprimido
    Guarda toda a informação necessária para a leitura de um bloco por um Thread.
*/

typedef struct argLeBloco {

    char* filename;
    int offset;
    char* bloco;
    int tamanho;
    int *tamDescomp;

}argLB;

/*
    Estrutura argDescodShaf
    - Nome do ficheiro
    - Bloco, string de caracteres a descodificar
    - Tamanho do bloco a descodificar
    - Tamanho do bloco depois de descodificado
    - Offset, onde começar a escrever os caracteres descodificados
    Guarda toda a informação necessária para a descodificação de um bloco por um Thread.
*/

typedef struct argDescodSahf{
    char *filename;
    char * bloco;
    int tamanho;
    int tamanhoDescod;
    int offset;
    cArray * codigos;
    char tipo;
}argDS;

/*
    União acederB
    - Contém 8 unsigned chars que correspondem aos 8 bits de um caracter
    Permite aceder aos 8 bits que compõem um caracter.
*/

typedef union
{
  struct
  {
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
    unsigned char bit8 : 1;
  }u;
  unsigned char byte;
}acederB;

void cortaSufixo(char *filename,char * nFilename,int nCorte);
int calculaOffset(int* tamanhos, int i);
int binToInt(char* seq, int tamanho);
void descompBlocoRle(argDB* arg);
void descodShaf(argDS * arg);
void exeNormal(char *filenameShaf);
void exeS(char *filenameShaf);
void exeR(char *filenameRle);
int get_maxBits (int * symbols, FILE *fp_cod );

#endif // ___logica_h___
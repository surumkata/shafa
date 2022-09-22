/*
    Gonçalo Pereira & José Gomes
    funções de leitura
*/
#include "logica.h"
#include <stdio.h>

#ifndef ___leitura_h___
#define ___leitura_h___


int* lerFC(char* filename, int* nBlocos);
void leBloco(argLB* arg);
int * lerCodNblocos(char* filenameCod, int * nBlocos,char *c);
void lerCodigos(char* filenameCod, cArray** codigos, int* tamanhos);
char **lerShaf(char* filenameShaf,int *tamanhosShaf);



#endif // ___leitura_h___
/*
    Gonçalo Pereira & José Gomes
    funções de leitura
*/

#include "leitura.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>



/*
    Função lerFC
    Recebe
    - nome de um ficheiro (.freq ou .cod)
    - apontador para inteiro
    Retorna
    - Array com os tamanhos dos blocos em bytes
*/

int* lerFC(char* filenameFC, int* nBlocos) {
    char tipo, c = 'R';
    int i = 0;
    FILE* fpFC;

    fpFC = fopen(filenameFC, "r");

    fgetc(fpFC);                              //le o @
    fscanf(fpFC, "%c", &tipo);                  //guardar o tipo do ficheiro rle|n
    fgetc(fpFC);                              //le o @
    fscanf(fpFC, "%d", nBlocos);                //guardar num de blocos
    fgetc(fpFC);                              //le o @

    int* tamanhos = (int*)malloc(sizeof(int) * (*nBlocos));

    do {
        fscanf(fpFC, "%d", &tamanhos[i]);
        fgetc(fpFC);
        while (c != '@') {
            c = fgetc(fpFC);
        }
        c = 'R';
        i++;
    } while (i < *nBlocos);

    fclose(fpFC);

    return tamanhos;
}



/*
    Função leBloco
    Recebe
    - Apontador para estrutura de argumentos
    Lê bloco de bytes

*/
void leBloco(argLB* arg) {
    FILE* fp;
    fp = fopen(arg->filename, "rb");
    fseek(fp, arg->offset, 0);                                      //aponta para o bloco a ler

    fread(arg->bloco, sizeof(char), (arg->tamanho), fp);            // Leitura do bloco

    char nreps;
    int tamDescomp=0;
    for (int i = 0; i < arg->tamanho; i++) {
        if (arg->bloco[i] != 0)                         // Se não for o inicio de uma sequencia
            tamDescomp++;
        else {
            i++;                                        // Letra a ser repetida
            nreps = arg->bloco[++i];                    // Número de repetições
            tamDescomp += nreps;
        }
    }
    (*arg->tamDescomp) = tamDescomp;
    fclose(fp);
}

/*
    Função lerCodNblocos
    Recebe
    - nome do ficheiro .cod
    - apontador para inteiro,para guardar numero de blocos
    Retorna
    - caracter que é o tipo do ficheiro (R|N)

*/
int * lerCodNblocos(char* filenameCod, int * nBlocos,char *c) {
    FILE* fpCod = fopen(filenameCod, "r");
    char buffer;
    int temp,simb[256];
    fgetc(fpCod);                               //le o @
    fscanf(fpCod,"%c",c);                       //guardar o tipo do ficheiro rle|n
    fgetc(fpCod);                               //le o @
    fscanf(fpCod, "%d",nBlocos);                //guardar numero de blocos
    fgetc(fpCod);                               //@
    int * max = malloc(sizeof(int)*(*nBlocos));

    for(int i=0;i<(*nBlocos);i++){
        fscanf(fpCod,"%d",&temp);                       //tamanho
        buffer = fgetc(fpCod);
        max[i] = get_maxBits(simb,fpCod);
    }
    fclose(fpCod);
    return max;
}

/*
    Função lerCodigos
    Recebe
    - Nome de um ficheiro .cod
    - Array de estruturas onde se guardará a descodificação do código e o seu tamanho, no indice do próprio código
    Guarda num array todos as descodificações dos códigos e os seus tamanhos.
*/


void lerCodigos(char* filenameCod, cArray** codigos, int* tamanhos) {
    char c = 'R', tipo, buffer;
    char seq[32];
    int nBlocos,codPosicao=0, codTamanho = 0, i=0, counter = 0;
    FILE* fpCOD = fopen(filenameCod, "r");
    fgetc(fpCOD);                              //le o @ só
    fscanf(fpCOD, "%c", &tipo);                  //guardar o tipo do ficheiro rle|n
    fgetc(fpCOD);                              //le o @
    fscanf(fpCOD, "%d",&nBlocos);                //guardar num de blocos
    fgetc(fpCOD);                              //le o @

    do {
        fscanf(fpCOD, "%d", &tamanhos[i]);
        buffer = fgetc(fpCOD);                           //le o @
        while (c != '@') {
            c = fgetc(fpCOD);
            if (c == ';' || c == '@'){
                if (codTamanho != 0) {
                    cArray elemento;
                    elemento.simbolo = codPosicao;
                    elemento.tamanho = codTamanho;
                    codigos[i][binToInt(seq, codTamanho)] = elemento;
                    codTamanho = 0;
                }
                codPosicao++;
            }
            else {
                 seq[codTamanho++] = c;
            }
        }
        c = 'R';
        i++;
        codPosicao=0;
    } while (i < nBlocos);
    fclose(fpCOD);
}

/*
    Função lerShaf
    Recebe
    - Nome de um ficheiro .shaf
    - Lista de tamanhos dos blocos
    Guarda numa lista todos os blocos de um determinado ficheiro .shaf
*/


char **lerShaf(char* filenameShaf,int *tamanhosShaf) {
    int nBlocos,i=0;
    char c;
    FILE * fp = fopen(filenameShaf,"rb");

    fgetc(fp);              //le @
    fscanf(fp,"%d",&nBlocos);   //le numero de blocos
    fgetc(fp);              //le @

    char **blocos = (char**)malloc(sizeof(char) * (nBlocos));

    do{
        fscanf(fp,"%d",&tamanhosShaf[i]);
        fgetc(fp);                          //le @

        blocos[i] = (char*)malloc(sizeof(char)*(tamanhosShaf[i]));

        fread(blocos[i],sizeof(char),tamanhosShaf[i],fp);

        fgetc(fp);                          //le @
        i++;
    }while(i<nBlocos);

    fclose(fp);
    return blocos;
}







/*
    Gonçalo Pereira & José Gomes
    funções de orocessamento e escrita
*/
#include "leitura.h"
#include "escrita.h"
#include "logica.h"
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>



/*
    Função leRle
    Recebe
    - nome do ficheiro rle
    - nome do ficheiro rle.freq
    Imprime no ficheiro TXT o conteúdo descomprimido do bloco.
*/

void processaRle(char* filenameRle, char* filenameFreq) {
    char* originalFilename = (char*)malloc(sizeof(char) * strlen(filenameRle) - 4);
    int nBlocos;
    int* tamanhos;

    cortaSufixo(filenameRle,originalFilename,5); //retira .rle

    tamanhos = lerFC(filenameFreq, &nBlocos);          // Array com tamanhos dos blocos

    char** blocos = (char**)malloc(sizeof(char*) * nBlocos);
    for (int i = 0; i < nBlocos; i++) {
        blocos[i] = (char*)malloc(sizeof(char) * (tamanhos[i]));      //alocar espaço tamanho do bloco
    }

    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t) * nBlocos);
    int* tamDescomp = (int*)malloc(sizeof(int)*nBlocos);


    for (int i = 0; i < nBlocos; i++) {
        argLB* arg = (argLB*)malloc(sizeof(argLB));
        arg->filename = filenameRle;
        arg->offset = calculaOffset(tamanhos, i);
        arg->bloco = blocos[i];
        arg->tamanho = tamanhos[i];
        arg->tamDescomp = &tamDescomp[i];
        pthread_create(&thread[i], NULL, (void*)leBloco, (void*)arg);
    }

    for(int i=0,rt;i<nBlocos;){
         rt = pthread_join(thread[i],NULL);
         if (rt == 0) {
             argDB* arg = (argDB*)malloc(sizeof(argDB));
             arg->filename = originalFilename;
             arg->offset = calculaOffset(tamDescomp, i);
             arg->bloco = blocos[i];
             arg->tamanho = tamanhos[i];
             pthread_create(&thread[i], NULL, (void*)descompBlocoRle, (void*)arg);
             i++;
         }
    }

    for (int i = 0, rt; i < nBlocos;) {
        rt = pthread_join(thread[i], NULL);
        if (rt == 0)
            i++;
    }
    mensagem.tamRLE = tamanhos;
    mensagem.tamDescompRLE = tamDescomp;
    mensagem.nBlocos = nBlocos;
    mensagem.ficheiroRLE = originalFilename;

    free(thread);
    free(blocos);
}

/*
    Função processaShaf
    Recebe
    - nome do ficheiro .cod
    - nome do ficheiro shaf
    - caracter (R|N), tipo do ficheiro
    Devolve
    - nome do ficheiro onde escreveu

    Lê um cod e guarda os codigos, lê um shaf e descodifica os blocos em paralelo
*/
char * processaShaf(char* filenameCod, char* filenameShaf,char *tipo) {
    char** blocos;
    char* escritaFilename = (char*)malloc(sizeof(char) * strlen(filenameCod) - 3);

    cArray** codigos;
    int* tamanhosCod, * tamanhosShaf, * maxBits;
    int nBlocos;

    cortaSufixo(filenameShaf,escritaFilename,6);                //cortar sufixo .shaf\0 ,obter nome onde vai escrever descodificado

    maxBits =  lerCodNblocos(filenameCod,&nBlocos,tipo);          // ler numero de blocos
    codigos = (cArray**)malloc(sizeof(cArray*) * nBlocos);
    tamanhosCod = (int*)malloc(sizeof(int) * nBlocos);
    tamanhosShaf = (int*)malloc(sizeof(int) * nBlocos);

    for(int i=0;i<nBlocos;i++){
        if(maxBits[i] > 8){
            codigos[i] = (cArray*)malloc(sizeof(cArray) * (int)pow(2,maxBits[i]));
        }else{
            codigos[i] = (cArray*)malloc(sizeof(cArray) * 256);
        }
    }

    for(int i=0;i<nBlocos;i++){
        cArray elemento = { -1,-1 };
        if(maxBits[i] > 8){
            for(int j=0;j<(int)pow(2,maxBits[i]);j++){
                codigos[i][j] = elemento;
            }
        }
        else{
            for(int j=0;j<256;j++){
                codigos[i][j] = elemento;
            }
        }
    }

    lerCodigos(filenameCod, codigos, tamanhosCod);
    blocos = lerShaf(filenameShaf,tamanhosShaf);

    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t) * nBlocos);

    for (int i = 0; i < nBlocos; i++) {
        argDS* arg = (argDS*)malloc(sizeof(argDS));
        arg->filename = escritaFilename;
        arg->bloco = blocos[i];
        arg->tamanho = tamanhosShaf[i];
        arg->tamanhoDescod = tamanhosCod[i];
        arg->offset = calculaOffset(tamanhosCod, i);
        arg->codigos = codigos[i];
        arg->tipo = (*tipo);
        pthread_create(&thread[i], NULL, (void*)descodShaf, (void*)arg);
    }

    for (int i = 0, rt; i < nBlocos;) {
        rt = pthread_join(thread[i], NULL);
        if (rt == 0)
            i++;
    }


    mensagem.tamSHAF = tamanhosShaf;
    mensagem.tamDescompSHAF = tamanhosCod;
    mensagem.nBlocos = nBlocos;
    mensagem.ficheiroSHAF = escritaFilename;

    free(codigos);
    free(maxBits);
    free(thread);

    return escritaFilename;
}



/*
    Função mensagemFim
    Recebe
    - tempo de execução
    - array com os tamanhos antes
    - array com tamanhos depois descompressão
    - número de blocos
    - nome do ficheiro produzido
    Imprime na consola mensagem no fim de execução do modulo
*/
void mensagemFim(int timer, int *tam, int *tamDescomp, int nBlocos, char* ficheiro){
    printf("====================SUCESSO====================\n");
    printf("Goncalo Pereira & Jose Gomes,a93168,a82418, MIEI/CD\n");
    printf("Modulo: D (descodificacao)\n");
    printf("Numero de blocos: %d\n",nBlocos);
    for(int i=0;i<nBlocos;i++){
        printf("Tamanho antes/depois do ficheiro gerado (bloco %d): %d/%d\n",i+1,tam[i],tamDescomp[i]);
    }
    printf("Tempo de execucao do modulo: %d segundos e %d milisegundos\n",timer/1000, timer%1000);
    printf("Ficheiro gerado: %s\n", ficheiro);



}

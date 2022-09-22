/**
@file freq.c
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freq.h"

// Funcao para inicializar uma matriz frqs a 0
void inic_f(frqs* f,int b){ // b = numero de blocos
    for(int i = 0; i<b ; i++){
        for(int j = 0; j<256; j++)
            f[i][j] = 0;
    }
}

//Funcao que cria uma string (chamada "nFilename") sem a ultima extensao (.rle por exemplo) da string dada
void editaNome(char * filename,char *nFilename){
    int flag=0;
    for(int i=0;flag<2;i++){
        if(filename[i] == '.')flag++;

        if(flag<2){
            nFilename[i] = filename[i];
        }

        else
           nFilename[i] = '\0';
    }
}

// Funcao que calcula as frequencias de um ficheiro .txt
void freq_txt(char* filename,int n_blocos, int s_blocos, int s_file, frqs* f){
    Info_blocos b; b.number = n_blocos; b.size = s_blocos;
    char c;
    int rep;
    FILE* fTxt;
    fTxt = fopen(filename,"rb");
    for(int b_atual = 0;b_atual < b.number; b_atual++)
    {
        if(b_atual < b.number - 1)
        {
            for(int i = 0; i < b.size;)  // percorremos todos os elementos do bloco definido e faz o calculo das frequencias de um ficheiro TXT
            {
                //printf("c : %d\n",c); //teste para ver o que e lido
                c = fgetc(fTxt);
                f[b_atual][c]++;
                i++;
            }
        }
        else
        {
            int s_ultimo_b = s_file - b_atual * b.size;
            for(int j = 0; j < s_ultimo_b; ) // percorremos todos os elementos do bloco, mas nao se sabe o tamanho do ultimo bloco (por nao ser igual aos outros), e faz o calculo das frequencias de um ficheiro TXT
            {
              // f[1][97] = 200; (no bloco 1 tem 200 aparicoes do char a(97 em ASCII))

                c = fgetc(fTxt);
                f[b_atual][c]++;
                j++;
                //printf("c : %d\n",c); //teste para ver o que e lido
            }
        }
    }
    fclose(fTxt);
}

// Funcao que calcula as frequencias de um ficheiro .rle
void freq_rle(char* filename,int n_blocos, int s_blocos, int s_file, frqs* f){

    Info_blocos b; b.number = n_blocos; b.size = s_blocos;
    int c, rep;
    FILE* fRle;
    fRle = fopen(filename,"rb");
    for(int b_atual = 0;b_atual < b.number; b_atual++)
    {
        if(b_atual < b.number - 1)
        {
            for(int i = 0; i < b.size;) // percorremos todos os elementos do bloco definido e faz o calculo das frequencias de um ficheiro RLE
            {
                //fread(&c,1,1,fRle);
                c = fgetc(fRle);
                //printf("b: %d, c: %d    %d\n",b_atual,c,i);
                if (c == 0) // verifica se encontra o primeiro 0 da compressao RLE
                {
                    c = fgetc(fRle);
                    rep = fgetc(fRle);
                    f[b_atual][c] += rep;
                    i+=rep;
                }
                else
                {
                    f[b_atual][c]++;
                    i++;
                }
            }
        }
        else
        {
            int s_ultimo_b = s_file - b_atual * b.size;
            for(int j = 0; j < s_ultimo_b; ) // percorremos todos os elementos do bloco, mas nao se sabe o tamanho do ultimo bloco (por nao ser igual aos outros), e faz o calculo das frequencias de um ficheiro RLE
            {
              // f[1][97] = 200; (no bloco 1 tem 200 aparicoes do char a(97 em ASCII))

                c = fgetc(fRle);
                //printf("c : %d\n",c); teste para ver o que e lido
                if (c == 0)
                {
                    c = fgetc(fRle);
                    rep = fgetc(fRle);
                    //printf("c : %d  d : %d\n",c,rep); //teste para ver o que e lido
                    f[b_atual][c] += rep;
                    j+=rep;
                }
                else
                {
                    f[b_atual][c]++;
                    j++;
                }
                //printf("b: %d, c: %d\n",b_atual,c);
            }
        }
    }
    fclose(fRle);
}

//funcao que utiliza os dados guardados na matriz frqs para criar o file .freq
void freq_file(char* filename, frqs* f, int tipof,int s_file, int n_blocos, int s_blocos, int* blocos_rle){
    Info_blocos b; b.number = n_blocos; b.size = s_blocos;
    char* fileFreq = strcat(filename,".freq");
    FILE* freq;
    freq = fopen(fileFreq,"wb");

    if(tipof == 0) // verifica se o calculo das frequencias provem do ficheiro TXT
    {
        fprintf(freq,"@N@");
    }
    else // caso contrario provem do ficheiro RLE
    {
        fprintf(freq,"@R@");

    }
    fprintf(freq,"%d",n_blocos); // imprime o numero de blocos no ficheiro FREQ

    //Declaracoes necessarias para escrever no ficheiro FREQ e navegar na matriz de frequencias
    int simbolo; int b_atual = 0;
    int s_restante = s_file - (b.size * (b.number-1));
    int elemento;

    while(b_atual<b.number){ // escreve as frequencias de cada simbolo (0 a 255) em todos os blocos

        if(b_atual != b.number - 1) // tamanho dos blocos, exceto o ultimo "@%d@"
            {
                if (tipof == 0)
                    fprintf(freq,"@%d@",s_blocos);
                else
                    fprintf(freq,"@%d@",blocos_rle[b_atual]);
            }
        else // tamanho do ultimo bloco  "@%d@"
            {
                if (tipof == 0)
                    fprintf(freq,"@%d@",s_restante);
                else
                    fprintf(freq,"@%d@",blocos_rle[b_atual]);
            }

        for(simbolo = 0; simbolo < 255; simbolo++)
            {
                //"%d;"
                elemento = f[b_atual][simbolo];
                if (elemento != 0)
                    fprintf(freq,"%d;",elemento);
                else
                    fprintf(freq,";");
            }
        elemento = f[b_atual][simbolo];
        if (elemento != 0)
            fprintf(freq,"%d",elemento); // ultimo simbolo escrito em separado por causa do ';'
        b_atual++;
    }

    fprintf(freq,"@0"); // colocar @0 para indicar que nao ha mais blocos

    fclose(freq);
}


// Recebe do main um ficheiro (rle ou txt) e calcula a frequencia, chamando outras funcoes auxiliares
void freq(char* filename, int n_blocos, int s_blocos, int tipof, int s_file, int* blocos_rle){
    fflush(stdout); // limpa buffers (estava a interferir com a inicializacao do n_blocos)
    frqs *f1 = (frqs*) malloc(n_blocos * sizeof(frqs));
    inic_f(f1,n_blocos);

    if (tipof == 0) // caso o ficheiro seja TXT
        freq_txt(filename, n_blocos, s_blocos, s_file,f1);

    else // caso o ficheiro seja RLE
        freq_rle(filename, n_blocos, s_blocos, s_file,f1);

    freq_file(filename,f1,tipof,s_file,n_blocos,s_blocos,blocos_rle);
    free(f1);
}

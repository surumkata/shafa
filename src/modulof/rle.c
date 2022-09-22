/**
@file rle.c
*/

#include <stdio.h>
#include <string.h>
#include "rle.h"

// Calcula o tamanho de um ficheiro em bytes
int tamanho_ficheiro (char* filename){
    FILE* fp;
    int tamanho;

    fp = fopen(filename, "rb");
    fseek(fp, 0L, SEEK_END);
    tamanho = ftell(fp);

    fclose(fp);

    return tamanho;
}

// Calcula o numero de blocos a usar para um certo ficheiro (em bytes)
Info_blocos n_blocos (int s_fp){

    Info_blocos info_blocos;

    if     (s_fp < 2*1024 * 1024)   info_blocos.size = 64*1024;        // assume-se que cada bloco tem 64 KBytes
    else if(s_fp < 16*1024 * 1024)  info_blocos.size = (640*1024);     // assume-se que cada bloco tem 640 KBytes
    else if(s_fp < 128*1024 * 1024) info_blocos.size = (8*1024*1024);  // assume-se que cada bloco tem 8 MBytes
    else                            info_blocos.size = (64*1024*1024); // assume-se que cada bloco tem 64 MBytes

    info_blocos.number = s_fp/info_blocos.size;
    if(s_fp - info_blocos.number * info_blocos.size > 1024)
         info_blocos.number++;

    return info_blocos;
}

// Escreve o numero de vezes que um dado caractere ocorre consecutivamente
int repeticoes (FILE* fp, char c, int s_bloco){
    int i;
    int cmp = fgetc(fp); // primeiro char lido
    for (i = 1 ; cmp == c && i < 255 && i < s_bloco; i++){
        // numero de repeticoes contadas nao pode exceder 255 nem o numero de elementos restantes do bloco atual
        cmp = fgetc(fp);
    }
    ungetc(cmp,fp);
    return i;
}

// Escreve no ficheiro .RLE a compressão de um caractere ou de uma sequência de caracteres
void comprime(FILE* fRLE,char c,int rep){
    if(rep < 4 && c != '0') // no caso do caractere repetir menos de 4 vezes consecutivamente
    {
        for(int i = 0; i<rep; i++)
            fprintf(fRLE,"%c",c);
    }
    else // no caso do caractere repetir igual ou superior a 4 vezes consecutivamente
    {
        int inicio = 0;
        fprintf(fRLE,"%c%c%c",inicio,c,rep);
    }
}

// Funcao principal para realizar a compressao RLE
void compressaoRLE(char* filename,Info_blocos b, int s_fp, char* filenameRLE, int* blocos_rle){
    strcpy(filenameRLE,filename);
    strcat(filenameRLE,".rle"); //escreve-se a terminacao do ficheiro RLE
    FILE* fp; FILE* fRLE;

    fp   = fopen(filename, "rb");
    fRLE = fopen(filenameRLE, "wb");
    int b_atual = 0;
    char c;
    int rep;

    for(;b_atual < b.number; b_atual++)
    {
        blocos_rle[b_atual] = 0; // iniciar o array que guarda o tamanho dos blocos RLE a 0
        if(b_atual < b.number - 1)
        {
            for(int i = 0; i < b.size;i += rep) // percorre todos os elementos do bloco com tamanho definido e faz a compressao RLE
            {
                c   = fgetc(fp);
                rep = repeticoes(fp,c,b.size - i);
                comprime(fRLE,c,rep);
                if(rep >= 4)
                    blocos_rle[b_atual] += 3;
                else
                    blocos_rle[b_atual] += rep;
            }
        }
        else
        {
            int s_ultimo_b = s_fp - b_atual * b.size; // tamanho do ultimo bloco

            for(int j = 0; j < s_ultimo_b; j+=rep) // percorremos todos os elementos do bloco, mas nao se sabe o tamanho do ultimo bloco (por nao ser igual aos outros), e faz a compressao RLE
            {
                c   = fgetc(fp);
                rep = repeticoes(fp,c,s_ultimo_b - j);
                comprime(fRLE,c,rep);
                if(rep >= 4)
                    blocos_rle[b_atual] += 3;
                else
                    blocos_rle[b_atual] += rep;
            }
        }
    }
    fclose(fRLE);
    fclose(fp);
}

// Calcula a taxa de compressao do ultimo bloco
int compressao_ult_bloco (char *filename,Info_blocos b,int s_fp){
    int tRle = 0;
    int tamanho;

    if(b.number == 1)
        tamanho = s_fp;
    else
        tamanho =b.size;

    FILE *fp;
    char c;
    int rep;
    fp   = fopen(filename, "rb");

    for(int i = 0; i < tamanho; i += rep)
    {
        c   = fgetc(fp);
        rep = repeticoes(fp,c,tamanho - i);

        if(c == '0' || rep >= 4)
            tRle += 3;
        else
            tRle += 1;
    }

    fclose(fp);


    float compressao = tamanho-tRle; // etapa 1 do calculo da taxa de compressao
    compressao /= tamanho;           // etapa 2 do calculo da taxa de compressao
    compressao *= 100;               // etapa 3 do calculo da taxa de compressao (em percentagem)
    //printf("Compressao do primeiro bloco: %f\ntamanho : %d e tamanhoRle : %d\n",compressao, tamanho, tRle);

    return compressao;
}

// Testa se vale a pena fazer a compressao RLE (compressão do primeiro bloco >=5%)
int verifica_compressao(char *filename,Info_blocos b,int s_fp){
    int compressao = compressao_ult_bloco (filename,b,s_fp);
    if (compressao >= 5) // se a compressao >= 5%, retorna 1
        return 1;
    return 0; // caso contrario retorna 0
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "codigo.c"
#include "listas.c"


int modulot(char *ficheiro){

    /* Inicio da contagem do tempo de execucao */
    clock_t t_inicial = clock();

    FILE* arqName = fopen(ficheiro, "r"); // Abre o ficheiro pretendido (read)

// LEITURA FICHEIRO .FREQ

    char compressao; // Tipo de compressao (N/R)
    int num_blocos;  // Numero de blocos
    fscanf(arqName,"@%c@%d@", &compressao, &num_blocos); // Le o tipo compressao e o numero de blocos


    int tam_blocos[num_blocos];      // Arreio com o tamanho dos blocos
    int lista_freq[num_blocos][256]; // Frequencias de todos os blocos


    for(int ba = 1; ba <= num_blocos; ba++){ // ba = bloco atual
        /* Leitura dos tamanhos */
        int tam_bloco;
        fscanf(arqName, "%d@", &tam_bloco);
        tam_blocos[ba-1] = tam_bloco; // Guarda o tamanho do respetivo bloco

        /* Leitura das frequencias */
        int f = 0; // Controlo do indice das 256 frequencias
        int freq; // Valor da frequencia
        char caracter = ';';

        for(;caracter == ';'; f++){
            fscanf(arqName, "%d", &freq);
            fscanf(arqName, "%c", &caracter);
            lista_freq[ba-1][f] = freq; //  Guarda a frequencia
        }
    }


// ORGANIZACAO DADOS OBTIDOS

    LISTA a[num_blocos];    // Arreio de listas, onde cada lista representa um bloco
    /* Coloca em listas as frequecias e respetivos simbolos, de cada bloco, de forma decrescente e ordenada */
    for(int i=0; i<num_blocos; i++) a[i] = arrayToListOrd(lista_freq[i]);


    /* Preenchimento do campo codigosSF das listas (bloco a bloco) */
    for(int b=0; b < num_blocos; b++){
        int simbolos = lengthL(a[b]);                 // Numero de simbolos do bloco b+1
        char codes[simbolos][256];                   // Arreio de strings para guardar os codigos SF
        int freqbloco[simbolos];                     // Arreio com as frequencias de cada simbolo do bloco b+1
        inicializeCodes(codes, simbolos);
        listToArray(a[b], freqbloco, simbolos);
        getSFCodes(freqbloco, codes, 0, simbolos-1, simbolos);
        storeSFCodes(a[b], codes);
    }


// CRIACAO FICHEIRO .COD

    /* Cria o ficheiro a ser gerado */
    int i;
    for(i=0; ficheiro[i] != 'f'; i++);
    ficheiro[i] = 'c';
    ficheiro[i+1] = 'o';
    ficheiro[i+2] = 'd';
    ficheiro[i+3] = '\0';
    FILE* ficheiro_gerado = fopen(ficheiro, "w"); // Abre o ficheiro gerado (write)
    /* Escreve o tipo de compressao e o numero de blocos no novo ficheiro */
    fprintf(ficheiro_gerado, "@%c@%d", compressao, num_blocos);
    /* Ciclo relacionado com o tamanho de cada bloco */
    for(int nb=0; nb<num_blocos; nb++){
        fprintf(ficheiro_gerado, "@%d@", tam_blocos[nb]);
        /* Ciclo relacionado com as frequencias de cada bloco */
        for(int simb=0; simb<256; simb++){
            LISTA temp = a[nb];
            /* Percorre a lista e verifica se o simbolo ocorre no bloco */
            while(temp && simb != temp->simbolo) temp = temp->proximo;
            /* Se a lista for != NULL significa que encontramos o simbolo */
            if(temp) fprintf(ficheiro_gerado, "%s", temp->codigoSF);
            /* Caso do ultimo codigo SF */
            if (simb != 255) fputc(';', ficheiro_gerado);
        }
    }
    /* Fim do ficheiro */
    fprintf(ficheiro_gerado, "@0");
    fclose(ficheiro_gerado);


    /* Fim da contagem do tempo de execucao */
    clock_t t_final = clock();
    double intrevalo_tempo = ((double) (t_final - t_inicial)*1000) / CLOCKS_PER_SEC; // milisegundos



// INTERFACE

    printf("Duarte Moreira(a93321), Lucas Carvalho(a93176), MIEI/CD, 1-jan-2021\n");
    printf("Modulo: t (calculo dos codigos dos simbolos)\n");
    printf("Numero de blocos: %d\n", num_blocos);
    printf("Tamanho dos blocos analisados no ficheiro de simbolos: ");
    imprimeTamahos(tam_blocos, num_blocos);
    printf("Tempo de execucao do modulo (milissegundos): %.0f\n", intrevalo_tempo);
    printf("Ficheiro gerado: %s\n", ficheiro);
    return 0;
}

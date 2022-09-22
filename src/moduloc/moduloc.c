/**
@file modulo.c
Ficheiro respons�vel por realizar todo o m�dulo C (codifica��o dum ficheiro de s�mbolos)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "moduloc.h"

// Escreve algumas informa��o no terminal sobre o m�dulo, como o n�mero de blocos, os tamanhos antes e depois do ficheiro gerado e tempo de execu��o.
void prompt (int num_blocks, int *size_before, int *size_after, double temp_exec, char* filename) {

    printf("Tiago Silva, a93277, Joao Barbosa, a93270, MIEI/CD, 1-jan-2021\n");
    printf("Modulo: c (codificacao dum ficheiro de simbolos)\n");
    printf("Numero de blocos: %d\n", num_blocks);
    for (int i = 0; i < num_blocks; i++){
        printf("Tamanho antes/depois do ficheiro gerado (bloco %d): %d/%d\n",(i+1),size_before[i],size_after[i]);
    }
    printf("Tempo de execucao do modulo (milissegundos): %.0f\n",temp_exec);
    printf("Ficheiro gerado: %s\n",filename);
}

// Guarda espa�o numa matriz para os s�mbolos utilizados por 8 offsets.
code ** malloc_matrix(code **matrix, int *symbols, int tamBytes){

    for(int symbol = 0; symbol < MAX_SYMBOLS; symbol++){
        if(symbols[symbol]){ // Guarda mem�ria apenas se o s�mbolo for utilizado. (symbols[symbol]=1)
            matrix[symbol] = malloc(MAX_OFFSETS * sizeof(code));

            for(int offset = 0; offset < MAX_OFFSETS; offset++){ // Guarda mem�ria para os 8 offsets.
                matrix[symbol][offset] = (code) malloc(sizeof(struct code_));
                matrix[symbol][offset]->value = malloc(tamBytes * sizeof(char));
                for(int i = 0; i < tamBytes; i++) matrix[symbol][offset]->value[i] = (char) 0;
            }
        }
    }

    return matrix;
}

// Liberta a mem�ria guardada anteriormente.
void free_matrix(code ** matrix, int* symbols){
    for(int symbol = 0; symbol < MAX_SYMBOLS; symbol++){
        if(symbols[symbol]){ // Liberta mem�ria apenas se o s�mbolo foi utilizado. (symbols[symbol]=1)
            for( int offset = 0; offset < MAX_OFFSETS; offset++) {
                free(matrix[symbol][offset]->value);
                free(matrix[symbol][offset]);
            }
            free(matrix[symbol]);
        }
    }
}


// Escreve num ficheiro .shaf o c�digo relativo a um bloco.
int write_shaf (code ** matrix, FILE* original, FILE *fp_fileshaf, int size, int sizeBytes){
    int offset = 0, nextbit, index, id_byte, id_shaf = 0, ascii_symbol;
    char symbol, current_char;

    int sizeofShaf = size * sizeBytes / sizeof(char);

    // Array que vai guardar a codifica��o do bloco a ser escrita no ficheiro do tipo shaf.
    char *shaf = malloc(size * sizeBytes);
    for(int x = 0; x < sizeofShaf; x++) shaf[x] = 0;

    // L� um bloco todo do ficheiro original e codifica-o com ajuda da matriz.
    for(int id_orig = 0; id_orig < size; id_orig++){
        ascii_symbol = fgetc(original); // L� um caracter do ficheiro original.
        if(ascii_symbol >= 0){ // Apenas uma seguran�a, pois utilizamos como um �ndice e j� obtivemos valores negativos.
            // Busca na matriz o �ndice de Byte e de Bit do simbolo.
            index = matrix[ascii_symbol][offset]->index;
            nextbit = matrix[ascii_symbol][offset]->nextbit;

            for(id_byte = 0; id_byte <= index; id_byte++){ // Realiza para todos os Bytes do s�mbolo.
                current_char = matrix[ascii_symbol][offset]->value[id_byte]; // Busca na matriz o valor para aquele Byte do simbolo.
                shaf[id_shaf+id_byte] = shaf[id_shaf+id_byte] | current_char; // Realiza um OR, guardando o valor no array shaf.
                }
        id_shaf+=index;
        offset = nextbit; // O offset fica com o valor do pr�ximo bit.
        }
    }
    if(nextbit) id_shaf += 1;


    fprintf(fp_fileshaf,"@%d@",id_shaf); // Escreve no ficheiro do tipo shaf o tamanho do bloco.
    fwrite(shaf, sizeof(char), id_shaf, fp_fileshaf); // Escreve no ficheiro do tipo shaf o bloco codificado.

    free(shaf); // Liberta a mem�ria do array shaf.
    return id_shaf; // Retorna o tamanho do bloco.
}

// Faz a 1� Leitura do ficheiro tipo cod, vendo quais s�o os s�mbolos utilizados e o maior numero de bits de um c�digo.

int get_maxBits (int *symbols, FILE *fp_cod ){
    int maxBits = 0, id_bits = 0, symbol = 0, block_not_over = 1; //true
    char c, c_ant = '@';
    while (block_not_over) { // Enquanto o bloco n�o acabar.
        c = fgetc(fp_cod);
        if (c == ';' || c == '@') {
            if (c_ant == '0' || c_ant == '1'){
                // Leu um c�digo para aquele s�mbolo.
                symbols[symbol] = 1; //true.
                if(maxBits < id_bits) maxBits = id_bits; // Encontrou um c�digo com o maior n�mero de bits at� agora.
                }
                // N�o leu um c�digo para aquele s�mbolo.
                else symbols[symbol] = 0; //false.
                if (c == '@') block_not_over = 0; //false.
                // O Bloco terminou
                else {
                    // O Bloco n�o terminou, logo adiciona 1 ao �ndice de simbolos e reseta o �ndice de bits.
                    symbol++;
                    id_bits = 0;
                }
            }
        if (c == '0' || c == '1') id_bits++; //Adiciona 1 ao �ndice de bits.
        c_ant = c;
    }
    return maxBits; // Retorna o maior n�mero de bits encontrado.
}

// Calcula o n�mero de Bytes necess�rios para o n�mero m�ximo de bits.
int calc_sizeBytes (int maxBits) {
    int sizeBytes;

    if (maxBits%8) sizeBytes = maxBits/8 + 1 + 1;
    else sizeBytes = maxBits/8 + 1;

    return sizeBytes;

}

// Faz a 2� leitura do ficheiro tipo cod, guardando na matriz os c�digos e os seus offsets.
void calc_matrix(code **matrix, int* symbols, int maxBits, FILE* fp_cod){
    int id_bits = 0, symbol = 0, block_not_over = 1; //true
    char c, c_ant = '@';
    char buffer[maxBits];

    while (block_not_over) { // Enquanto o bloco n�o acabar
        c = fgetc(fp_cod);
        if (c == ';' || c == '@') {
            if (c_ant == '0' || c_ant == '1'){
                // Leu um c�digo para aquele s�mbolo.
                for(int offset = 0; offset < MAX_OFFSETS; offset++){ // Guarda o c�digo para os 8 offsets.

                    int value, exp, index, count, i;

                    for(i = offset, index = 0, count = 0; count < id_bits; index ++){

                        for (value = 0; count < id_bits && i < 8; i++){
                            exp = 7 - i;
                            if(buffer[count] == '1') value += (int) pow(2.0,(double)exp);
                            count++;
                        }
                        i = 0;
                        matrix[symbol][offset]->value[index]=value;
                    }

                    int nextbit = (id_bits+offset)%8;
                    matrix[symbol][offset]->nextbit=nextbit; // Guarda o pr�ximo bit do c�digo.
                    if (nextbit) matrix[symbol][offset]->index=index-1;
                    else matrix[symbol][offset]->index=index;
                    // Guarda o index do c�digo.
                }
            }
            if(c == '@') block_not_over = 0; //false
            // O Bloco terminou
            else {
                // O Bloco n�o terminou, logo adiciona 1 ao �ndice de simbolos e reseta o �ndice de bits.
                symbol++;
                id_bits = 0;
            }
        }
        if (c == '0' || c == '1') {
            // Adiciona 1 ao �ndice de bits, e adiciona o '0' ou o '1' ao buffer.
            buffer[id_bits] = c;
            id_bits++;
        }
        c_ant = c;
    }
}

//Realiza todo o m�dulo c.
void moduloc(char *filename){

    clock_t t_start = clock();
    int num_blocks;

    // Cria os nome dos ficheiros com .cod e .shaf atrav�s do nome do ficheiro original.
    char filename_cod[50]; char filename_shaf[50];char buffer[1];

    strcpy(filename_cod, filename); strcat(filename_cod,".cod");
    strcpy(filename_shaf, filename); strcat(filename_shaf,".shaf");

    // Inicializa os apontadores e abre todos os ficheiros necess�rios para decorrer o m�dulo c.
    FILE *fp1_cod, *fp2_cod, *fp_orig, *fp_shaf;

    fp1_cod = fopen(filename_cod,"r"); // Abre o ficheiro do tipo cod para uma primeira leitura.
    fp2_cod = fopen(filename_cod,"r"); // Abre o ficheiro do tipo cod para uma segunda leitura.
    fp_orig = fopen(filename,"rb"); // Abre o ficheiro original e l� o em bin�rio.
    fp_shaf = fopen(filename_shaf,"wb+"); // Cria um ficheiro do tipo shaf em bin�rio para a escrita da codifica��o.

    // L� o n�mero de blocos escrito no ficheiro do tipo cod.
    fscanf(fp1_cod,"@%c@",buffer);
    fscanf(fp1_cod,"%d@",&num_blocks);

    fscanf(fp2_cod,"@%c@");
    fscanf(fp2_cod,"%d@");

    fprintf(fp_shaf,"@%d",num_blocks);

    int block_size[num_blocks]; // Array com os tamanhos dos blocos do ficheiro original.
    int block_size_shaf[num_blocks]; // Array com os tamanhos dos blocos do ficheiro shaf.
    int symbols[MAX_SYMBOLS]; // Array que diz quais s�mbolos ser�o codificados.

    code** matrix = malloc(MAX_SYMBOLS * sizeof(code*));

    for(int id_block = 0; id_block < num_blocks; id_block++){

        // L� o tamanho do bloco.

        fscanf(fp1_cod,"%d@", &block_size[id_block]);
        fscanf(fp2_cod,"%d@");

        // Faz uma primeira leitura do ficheiro tipo cod naquele bloco e descrobre o n�mero m�ximo de bits que um c�digo pode ter.
        int maxBits = get_maxBits(symbols, fp1_cod);

        // Calcula o n�mero de Bytes necess�rio a partir do n�mero m�ximo de bits.
        int sizeBytes = calc_sizeBytes(maxBits);

        // Guarda espa�o para a matriz dependendo do n�mero de Bytes a guardar e dos simbolos utilizados.
        malloc_matrix(matrix, symbols, sizeBytes);

        // A partir de uma segunda leitura do tipo cod guarda os c�digos dentro da matriz (calculando j� os 8 offsets).
        calc_matrix(matrix, symbols, maxBits, fp2_cod);

        // Codifica o original a partir dos c�digos da matriz e escreve-os no tipo shaf, retorna tamb�m o tamanho do bloco j� codificado.
        block_size_shaf[id_block] = write_shaf(matrix,fp_orig,fp_shaf, block_size[id_block], sizeBytes-1);

        // Liberta a mem�ria guardada para a matriz anteriormente, para que possa ser utilizada num pr�ximo bloco.
        free_matrix(matrix,symbols);
    }

    // Liberta completamente a matriz.
    free(matrix);

    // Fecha todos os ficheiros anteriormente abertos.
    _fcloseall();

    // Calcula em milisegundos o tempo de execu��o do programa.
    clock_t t_final = clock();
    double temp_exec = ((double) (t_final - t_start)*1000) / CLOCKS_PER_SEC;

    // Escreve no terminal algumas informa��es relevantes sobre a execu��o do m�dulo c.
    prompt(num_blocks, block_size, block_size_shaf, temp_exec, filename_shaf);
}

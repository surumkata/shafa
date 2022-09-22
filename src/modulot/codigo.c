#include <stdlib.h>
#include <stdio.h>
#include "codigo.h"
#include "listas.h"


int soma(int freq[], int i, int j){
    int sum;
    for(sum=0; i<=j; i++)
        sum = sum + freq[i];
    return sum;
}


int getBestDivision(int freq[], int i, int j){
    int div = i;
    int g1 = 0;
    int total, mindif, dif;
    total = mindif = dif = soma(freq, i, j);
    while(dif == mindif){
        g1 = g1 + freq[div];
        dif = abs(2*g1 - total);
        if(dif < mindif){
            div = div + 1;
            mindif = dif;
        }
        else dif = mindif + 1;
    }
    return div-1;
}


void inicializeCodes(char codes[][256], int num_simb){
    for(int i=0; i<num_simb; i++)
        codes[i][0] = '\0';
}


void addBitCode(char b, char codes[][256], int start, int end){
    while(start <= end){
        int i;
        for(i=0; codes[start][i] != '\0'; i++);
        codes[start][i] = b;
        codes[start][i+1] = '\0';
        start++;
    }
}


void getSFCodes(int freq[], char codes[][256], int start, int end, int size){
    int div = 0;
    /* Casos em que o arreio freq tem tamanho >1 */
    if (end != -1 && start != end){
        div = getBestDivision(freq, start, end);
        addBitCode('0', codes, start, div);
        addBitCode('1', codes, div+1, end);
        getSFCodes(freq, codes, start, div, size);
        getSFCodes(freq, codes, div+1, end, size);
    }
    /* Caso em que o arreio freq tem tamanho 1 */
    if (size == 1)
        addBitCode('0', codes, start, div);
}


void imprimeTamahos(int tam_blocos[], int num_blocos){
    for(int k=0; k < num_blocos-1; k++)
        printf("%d/", tam_blocos[k]);
    printf("%d bytes\n", tam_blocos[num_blocos-1]);
}


void storeSFCodes(LISTA l, char codes[][256]){
    LISTA temp = l;
    for(int i=0; temp; i++){
        int j;
        for(j=0; codes[i][j] != '\0'; j++)
            temp->codigoSF[j] = codes[i][j];
        temp->codigoSF[j] = '\0';
        temp = temp->proximo;
    }
}

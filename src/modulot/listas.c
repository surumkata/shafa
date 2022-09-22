#include <stdio.h>
#include <stdlib.h>
#include "listas.h"


LISTA criar_lista(int f, int s){
    LISTA l = malloc(sizeof(struct nodo));
    l->frequencia = f;
    l->simbolo = s;
    l->proximo = NULL;
    return l;
}


int lengthL(LISTA l){
    int size;
    for(size=0; l; size++) l = l->proximo;
    return size;
}


void listToArray(LISTA l, int arr[], int size){
    int i;
    for (i=0; i<size; i++){
        arr[i] = l->frequencia;
        l = l->proximo;
    }
}


void insertOrd(LISTA *l, LISTA novo){
    LISTA temp;
    /* Inserir na cabeça da lista */
    if (*l == NULL || (*l)->frequencia <= novo->frequencia){
        novo->proximo = *l;
        *l = novo;
    }
    else{
        /* Inserir no meio */
        temp = *l;
        while(temp->proximo != NULL && temp->proximo->frequencia > novo->frequencia){
            temp = temp->proximo;
        }
        novo->proximo = temp->proximo;
        temp->proximo = novo;
    }
}


LISTA arrayToListOrd(int freq[]){
    LISTA l = NULL;
    for(int i=255; i>=0; i--){
            /* Nao interessa guardar simbolos que nao occorram */
            if(freq[i] != 0){
                LISTA nodo = criar_lista(freq[i], i);
                insertOrd(&l, nodo);
            }
    }

    return l;
}

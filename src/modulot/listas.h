#ifndef LISTAS_H_INCLUDED
#define LISTAS_H_INCLUDED


typedef struct nodo {
    int frequencia;
    int simbolo;
    char codigoSF[256];
    struct nodo *proximo;
} *LISTA;


/**
  * brief Cria um nodo/lista
  * @param Frequecia
  * @param Simbolo
  * @returns Nodo
  */
LISTA criar_lista(int f, int s);

/**
  * brief Calcula o comprimento de uma lista
  * @param Lista
  * @returns comprimento
  */
int lengthL(LISTA l);

/**
  * brief Converte uma lista num arreio
  * @param Lista
  * @param Arreio
  * @param Tamanho da lista
  */
void listToArray(LISTA l, int arr[], int n);


/**
  * brief Insere um nodo numa lista ordenada de forma decrescente
  * @param Lista
  * @param Nodo
  */
void insertOrd(LISTA *l, LISTA novo);


/**
  * brief Converte um array numa lista, de forma ordenada e decrescente
  * @param Arreio
  */
LISTA arrayToListOrd(int arr[]);

#endif //LISTAS_H_INCLUDED

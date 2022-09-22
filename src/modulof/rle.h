/**
@file rle.h
*/

#include <stdio.h>

/**
 * \brief Refere-se à informação dos blocos
 */
typedef struct info_blocos
{ /** refere-se ao número de blocos;*/
  int number;
  /** refere-se ao tamanho dos blocos;*/
  int size;
} /** Refere-se à informação dos blocos*/ Info_blocos;

/**
 * \brief Função que calcula o tamanho de um ficheiro em bytes
 * @param filename Ficheiro a usar
 * @return Tamanho do ficheiro
 */
int tamanho_ficheiro (char* filename);

/**
 * \brief Função que calcula o numero de blocos a usar para um certo ficheiro (em bytes)
 * @param s_fp Tamanho do ficheiro
 * @return Struct Número + Tamanho dos blocos
 */
Info_blocos n_blocos (int s_fp);

/**
 * \brief Função que escreve o número de vezes que um dado caractere ocorre consecutivamente
 * @param fp Ficheiro a usar
 * @param c Caractere a usar para calcular
 * @param s_bloco Tamanho do bloco
 * @return Número de vezes que o caractere aparece consecutivamente
 */
int repeticoes (FILE* fp, char c, int s_bloco);

/**
 * \brief Função que escreve no ficheiro .RLE a compressão de um caractere ou de uma sequência de caracteres
 * @param fRLE Ficheiro a usar
 * @param c Caractere a usar para calcular
 * @param rep Número de vezes que um caractere aparece consecutivamente
 */
void comprime(FILE* fRLE,char c,int rep);

/**
 * \brief Função principal para realizar a compressao RLE
 * @param filename Ficheiro a usar
 * @param b Struct com número + tamanho dos blocos
 * @param s_fp Tamanho do ficheiro
 * @return Ficheiro Contem a sintaxe de um ficheiro.rle
 */
void compressaoRLE(char* filename,Info_blocos b, int s_fp, char* filenameRLE, int* blocos_rle);


/**
 * \brief Função que retorna a compressao do ultimo bloco
 * @param filename Ficheiro a usar
 * @param b Struct com número + tamanho dos blocos
 * @param s_fp Tamanho do ficheiro
 * @return Compressao do ultimo bloco
 */
int compressao_ult_bloco (char *filename,Info_blocos b,int s_fp);

/**
 * \brief Função que testa se vale a pena fazer a compressao RLE (compressão do primeiro bloco >=5%)
 * @param filename Ficheiro a usar
 * @param b Struct com número + tamanho dos blocos
 * @param s_fp Tamanho do ficheiro
 * @return Booleano que retorna 1 se valer a pena fazer a compressão, caso contrário retorna 0
 */
int verifica_compressao(char *filename,Info_blocos b,int s_fp);

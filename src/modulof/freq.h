/**
@file freq.h
*/

#include <stdio.h>

/**
 * \brief Array que guarda as frequencias de cada elemento em cada bloco
 */
typedef int frqs[256];

/**
 * \brief Função para inicializar uma matriz frqs
 * @param f Matriz frqs
 * @param b Número de blocos
 */
void inic_f(frqs* f,int b);

/**
 * \brief Função que cria uma string sem a ultima extensao (.rle por exemplo) da string dada
 * @param filename Ficheiro antigo
 * @param nFilename Ficheiro novo
 */
void editaNome(char * filename,char *nFilename);


/**
 * \brief Função que calcula as frequências de um ficheiro .txt
 * @param filename Ficheiro a usar
 * @param b Struct Tamanho+Número de blocos
 * @param s_file Tamanho do ficheiro
 * @return Ficheiro .txt com as frequências
 */
void freq_txt(char* filename,int n_blocos, int s_blocos, int s_file, frqs* f);

/**
 * \brief Função que calcula as frequências de um ficheiro .rle
 * @param filename Ficheiro a usar
 * @param b Struct Tamanho+Número de blocos
 * @param s_file Tamanho do ficheiro
 * @return Ficheiro .rle com as frequências
 */
void freq_rle(char* filename,int n_blocos, int s_blocos,int s_file, frqs* f);

/**
 * \brief Função que utiliza os dados guardados na matriz frqs para criar o ficheiro .freq
 * @param filename Ficheiro antigo
 * @param f matriz com as frequências de cada elemento e o respetivo simbolo
 * @param tipof Int que representa o tipo de ficheiro recebido
 * @param s_file Tamanho do ficheiro
 * @param b Struct Tamanho+Número de blocos
 */
void freq_file(char* filename, frqs* f, int tipof,int s_file, int n_blocos, int s_blocos, int* blocos_rle);

/**
 * \brief Função principal para calcular as frequências que faz o cálculo consoante a receção de um ficheiro .txt ou um .rle
 * @param filename Ficheiro antigo
 * @param b Struct Tamanho+Número de blocos
 * @param tipof Int que representa o tipo de ficheiro recebido
 * @param s_file Tamanho do ficheiro
 */
void freq(char* filename, int n_blocos, int s_blocos, int tipof, int s_file, int* blocos_rle);

/**
@file modulo.h
Ficheiro respons�vel por realizar todo o m�dulo C (codifica��o dum ficheiro de s�mbolos).
*/

#ifndef ___MODULOC_H___
#define ___MODULOC_H___

/**
\brief Refere-se ao n�mero m�ximo de s�mbolos.
*/
int MAX_SYMBOLS = 256;

/**
\brief Refere-se ao n�mero m�ximo de offsets.
*/
int MAX_OFFSETS = 8;


/**
 * \brief Refere-se � estrutura de code.
 */
typedef struct code_{
    char* value; // Valor do s�mbolo por Bytes.
    int nextbit; // �ndice do bit.
    int index; // �ndice do Byte.
}*code;

/**
 * \brief Fun��o que escreve no terminal algumas informa��es relevantes sobre o m�dulo.
 * @param num_blocks N�mero de blocos que o ficheiro est� dividido.
 * @param size_before Tamanho dos blocos antes da codifica��o.
 * @param size_after Tamanho dos blocos depois da codifica��o.
 * @param temp_exec Tempo de execu��o do m�dulo em milisegundos.
 * @param filename Nome do ficheiro gerado neste m�dulo.
 */
void prompt (int num_blocks, int *size_before, int *size_after, double temp_exec, char* filename);


/**
 * \brief Fun��o que guarda espa�o em mem�ria dentro da Matriz para 8 offsets de code por simbolos uilizados.
 * @param matrix Matriz onde vai guardar a mem�ria.
 * @param symbols Array booleano que diz quais dos 256 s�mbolos utilizados (symbols[x]=1 -> x utilizado | symbols[x]=0 -> x n�o utilizado).
 * @param tamBytes Tamanho de Bytes necess�rios que code ir� precisar.
 * @return Matriz.
 */
code ** malloc_matrix(code **matrix, int *symbols, int tamBytes);


/**
 * \brief Fun��o que liberta espa�o na mem�ria da matriz.
 * @param matrix Matriz a ser libertada da mem�ria.
 * @param symbols Array booleano que diz quais dos 256 s�mbolos utilizados
 */
void free_matrix(code ** matrix, int* symbols);


/**
 * \brief Fun��o que realiza a 1� leitura do ficheiro do tipo cod.
 * @param symbols Array booleano que diz quais dos 256 s�mbolos utilizados.
 * @param fp_cod Apontador para o ficheiro do tipo cod.
 * @return N�mero de bits m�ximo.
 */
int get_maxBits (int *symbols, FILE *fp_cod );

/**
 * \brief Fun��o que calcula o n�mero de Bytes m�ximo atrav�s do n�mero de bits m�ximo.
 * @param maxBits N�mero m�ximo de bits.
 * @return N�mero de Bytes.
 */
int calc_sizeBytes (int maxBits);

/**
 * \brief Fun��o que realiza a 2� leitura do ficheiro tipo cod.
 * @param matrix Matriz.
 * @param symbols Array booleano que diz quais dos 256 s�mbolos utilizados.
 * @param maxBits N�mero m�ximo de bits.
 * @param fp_cod Apontador para o ficheiro do tipo cod.
 */
void calc_matrix(code **matrix, int* symbols, int maxBits, FILE* fp_cod);

/**
 * \brief Fun��o que obt�m o jogador atual.
 * @param matrix Matriz
 * @param original Array com um bloco do ficheiro original para ser codificado.
 * @param filename Nome do ficheiro a ser gerado.
 * @param size Tamanho do bloco.
 * @return Tamanho do bloco depois de codificado.
 */
int write_shaf (code ** matrix, FILE* original, FILE* filename, int size, int sizeBytes);

/**
 * \brief Fun��o que realiza todo o m�dulo c.
 * @param filename Nome do ficheiro recebido.
 */
void moduloc(char *filename);

#endif //___LISTAS_H___

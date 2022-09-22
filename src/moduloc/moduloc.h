/**
@file modulo.h
Ficheiro responsável por realizar todo o módulo C (codificação dum ficheiro de símbolos).
*/

#ifndef ___MODULOC_H___
#define ___MODULOC_H___

/**
\brief Refere-se ao número máximo de símbolos.
*/
int MAX_SYMBOLS = 256;

/**
\brief Refere-se ao número máximo de offsets.
*/
int MAX_OFFSETS = 8;


/**
 * \brief Refere-se à estrutura de code.
 */
typedef struct code_{
    char* value; // Valor do símbolo por Bytes.
    int nextbit; // Índice do bit.
    int index; // Índice do Byte.
}*code;

/**
 * \brief Função que escreve no terminal algumas informações relevantes sobre o módulo.
 * @param num_blocks Número de blocos que o ficheiro está dividido.
 * @param size_before Tamanho dos blocos antes da codificação.
 * @param size_after Tamanho dos blocos depois da codificação.
 * @param temp_exec Tempo de execução do módulo em milisegundos.
 * @param filename Nome do ficheiro gerado neste módulo.
 */
void prompt (int num_blocks, int *size_before, int *size_after, double temp_exec, char* filename);


/**
 * \brief Função que guarda espaço em memória dentro da Matriz para 8 offsets de code por simbolos uilizados.
 * @param matrix Matriz onde vai guardar a memória.
 * @param symbols Array booleano que diz quais dos 256 símbolos utilizados (symbols[x]=1 -> x utilizado | symbols[x]=0 -> x não utilizado).
 * @param tamBytes Tamanho de Bytes necessários que code irá precisar.
 * @return Matriz.
 */
code ** malloc_matrix(code **matrix, int *symbols, int tamBytes);


/**
 * \brief Função que liberta espaço na memória da matriz.
 * @param matrix Matriz a ser libertada da memória.
 * @param symbols Array booleano que diz quais dos 256 símbolos utilizados
 */
void free_matrix(code ** matrix, int* symbols);


/**
 * \brief Função que realiza a 1º leitura do ficheiro do tipo cod.
 * @param symbols Array booleano que diz quais dos 256 símbolos utilizados.
 * @param fp_cod Apontador para o ficheiro do tipo cod.
 * @return Número de bits máximo.
 */
int get_maxBits (int *symbols, FILE *fp_cod );

/**
 * \brief Função que calcula o número de Bytes máximo através do número de bits máximo.
 * @param maxBits Número máximo de bits.
 * @return Número de Bytes.
 */
int calc_sizeBytes (int maxBits);

/**
 * \brief Função que realiza a 2º leitura do ficheiro tipo cod.
 * @param matrix Matriz.
 * @param symbols Array booleano que diz quais dos 256 símbolos utilizados.
 * @param maxBits Número máximo de bits.
 * @param fp_cod Apontador para o ficheiro do tipo cod.
 */
void calc_matrix(code **matrix, int* symbols, int maxBits, FILE* fp_cod);

/**
 * \brief Função que obtém o jogador atual.
 * @param matrix Matriz
 * @param original Array com um bloco do ficheiro original para ser codificado.
 * @param filename Nome do ficheiro a ser gerado.
 * @param size Tamanho do bloco.
 * @return Tamanho do bloco depois de codificado.
 */
int write_shaf (code ** matrix, FILE* original, FILE* filename, int size, int sizeBytes);

/**
 * \brief Função que realiza todo o módulo c.
 * @param filename Nome do ficheiro recebido.
 */
void moduloc(char *filename);

#endif //___LISTAS_H___

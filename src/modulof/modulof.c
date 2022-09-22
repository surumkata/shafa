
/**
@file main.c
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "rle.c"
#include "freq.c"

void imprime_prompt (char* filename, float texec, int create_rle, int size_txt, Info_blocos b, int* blocos_rle){

    FILE *fp;
    fp = fopen(filename, "rb");

    char filenameRLE[strlen(filename) + 10], filenameRLE_FREQ[strlen(filename) + 10]; // referem-se aos ficheiros RLE e RLE_FREQ, respetivamente

    int comp_lastBlock = compressao_ult_bloco(filename, b, size_txt); // compressao do ultimo bloco

    printf("--------------------------------------------------------------------------------------------------- \n");
    printf("----------------------- Modulo f : calculo das frequencias dos simbolos --------------------------- \n");
    printf("--------------------------------------------------------------------------------------------------- \n");
    printf("Goncalo Braz Afonso,  a93178, MIEI/CD, 1-jan-2021 \n");
    printf("Simao Pedro Sa Cunha, a93262, MIEI/CD, 1-jan-2021 \n");
    printf("Numero de blocos: %d \n", b.number);
    printf("Tamanho dos blocos analisados no ficheiro original: ");

    if (b.number > 1)
        printf("%d/%d bytes \n", b.size, size_txt - (b.size * (b.number-1) ));
    else
        printf("%d bytes \n",size_txt - (b.size * (b.number-1) ));



    if (create_rle == 1) //caso tenha sido criado um ficheiro RLE
        {
            strcpy(filenameRLE,filename);
            strcat(filenameRLE,".rle"); //cria nova string com nome do ficheiro original + .rle

            int size_rle = tamanho_ficheiro(filenameRLE);
            printf("Compressao RLE: %s (%d%% de compressao) \n",filenameRLE, comp_lastBlock );
            printf("Tamanho dos blocos analisados no ficheiro RLE (bytes): ");

            for(int b_atual =0; b_atual< b.number; b_atual++) printf("%d ",blocos_rle[b_atual]); //ciclo que imprime o tamanho de todos os blocos RLE

            printf("\nTempo de execucao do modulo (milissegundos): %.0f \n", texec*1000);

            strcpy(filenameRLE_FREQ,filenameRLE);
            strcat (filenameRLE_FREQ, ".freq");  //cria nova string com nome do ficheiro + .freq
            printf("Ficheiros gerados: %s, %s \n", filenameRLE, filenameRLE_FREQ); // escrever os nomes dos ficheiros corretamente
        }
    else // caso nao tenha sido criado o ficheiro RLE, apenas indica a criacao do ficheiro .FREQ
        {
            printf("Tempo de execucao do modulo (milissegundos): %.0f \n", texec*1000);

            strcpy(filenameRLE_FREQ,filename);
            strcat (filenameRLE, ".freq");  //cria nova string com nome do ficheiro original .freq
            printf("Ficheiros gerados: %s \n", filenameRLE_FREQ);
        }
    printf("--------------------------------------------------------------------------------------------------- \n");

    fclose(fp);
}


//./main.exe teste.txt -m f
int modulof (char* filename, char* compressao){

    char filenameRLE[strlen(filename) + 10]; // espaco suficiente para adicionar duas extensoes no nome do ficheiro
    int create_rle = 0;

    clock_t start = clock(); //tempo começa a contar no inicio do programa
    int s_fp = tamanho_ficheiro (filename);
    Info_blocos b = n_blocos (s_fp);
    int blocos_rle[b.number]; // array para guardar o tamanho dos blocos no ficheiro rle
    //printf("Tamanho = %d.\n", s_fp);

    if (s_fp < 1024 && compressao[0] != 'r') // se o ficheiro tiver menos de 1kb nao sofre compressao
    {
        printf("Ficheiro demasiado pequeno. O programa nao pode ser executado\n");
        return 0;
    }

    else
    {
        if (compressao[0] == 'r' || verifica_compressao(filename,b,s_fp) == 1 )
            {
                // se a compressao do primeiro bloco for menor que 5% nao sofre compressao a nao ser que seja forcada pelo utilizador
                create_rle = 1;
                //printf("Numero de blocos = %d.\nTamanho de blocos = %d\n", b.number, b.size);
                fflush(stdout); // limpar o buffer
                compressaoRLE(filename,b,s_fp,filenameRLE, blocos_rle);
                freq(filenameRLE, b.number, b.size, 1,s_fp,blocos_rle);

            }
        else
            {
                printf("Compressao do primeiro bloco menor do que 5 porcento. O ficheiro nao sera comprimido, apenas criar-se-a o ficheiro das frequencias.\n");
                //printf("Numero de blocos = %d.\nTamanho de blocos = %d\n", b.number, b.size);
                fflush(stdout); // limpar o buffer
                freq(filename,b.number, b.size,0,s_fp,blocos_rle);
            }
    }
    clock_t end = clock(); // tempo acaba de contar no fim do programa
    //printf("Tempo de execucao: %.6f segundos.\n", (double)(end - start) / CLOCKS_PER_SEC);

    //printf("FIM DO PROGRAMA\n");
    imprime_prompt(filename,(double)(end - start) / CLOCKS_PER_SEC, create_rle, s_fp,b , blocos_rle); // numero zero simboliza o tempo de execução (a corrigir)

    return 0;
}


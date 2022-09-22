/*
    Gonçalo Pereira & José Gomes
    Main
*/
#include <stdlib.h>
#include "escrita.c"
#include "leitura.c"
#include "logica.c"
#include <time.h>

/*
    INPUT: ficheiro(.shaf ou.rle)  nada|s|r
*/
int modulod(char* filename,char* argumento){


    if(argumento == 'r') exeR(filename);                  //Funciona most of the time :')
    else if (argumento == 's') exeS(filename);                   //Funciona
         else exeNormal(filename);


return 0;
}



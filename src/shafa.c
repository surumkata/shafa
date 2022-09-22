#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulot/modulot.c"
#include "modulof/modulof.c"
#include "moduloc/moduloc.c"
#include "modulod/modulod.c"


int main (int argc, char **argv){
    int contParam; char modulo[1] = ""; char argumento[1] = "";
    char* filename = argv[1];

    for(contParam = 1; contParam < argc; contParam++)
    {
        if(strcmp(argv[contParam], "-m") == 0)
        {
            strncpy(modulo,argv[contParam+1],1);
            contParam++;
        }
        else
        {
            if(strcmp(argv[contParam], "-c") == 0 || strcmp(argv[contParam], "-d") == 0)
            {
                strncpy(argumento,argv[contParam+1],1);
                contParam++;
            }
        }
    }

    if(modulo[0] == 't')
    {
        modulot(filename);
    }
    if(modulo[0] == 'f')
    {
        modulof(filename,argumento);
    }
    if(modulo[0] == 'c')
    {
        moduloc(filename);
    }
    if(modulo[0] == 'd')
    {
        modulod(filename,argumento);
    }

    return 0;
}

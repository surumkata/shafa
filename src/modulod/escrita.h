/*
    Gonçalo Pereira & José Gomes
    funções de orocessamento e escrita
*/



#ifndef ___escrita_h___
#define ___escrita_h___


/* 
    estretura para passagem de argumentos para multi threading na escrita paralela
*/



void processaRle(char* filename, char* filenameFreq);
void mensagemFim(int timer, int* tam, int* tamDescomp, int nBlocos, char* ficheiro);
char * processaShaf(char* filenameCod, char* filenameShaf,char *tipo);



#endif // ___escrita_h___
#include "reader.h"

char word[24];

int readWord(){

    char *filename = "/home/lcom/labs/proj/src/game/wordList.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("no file sucker\n");
        char CWD[256];
        getcwd(CWD,sizeof(CWD));
        printf("%s", CWD);
        return 1;
    }

    fgets(word, sizeof(word), file);
    int words = atoi(word);
    int n = 1 + rand() % words;

    while (n) {
        fgets(word, sizeof(word), file);
        word[strlen(word)-2] = '\0';
        n--;
    }

    fclose(file);
    
    return 0;
}

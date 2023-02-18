#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int valid(const char *arg) {
    return atoi(arg) || (arg[0] == '0' && strlen(arg) == 1);
}

int main(int argc, char *argv[])
{
    printf("Lista dos argumentos passados\n");
    for (int i = 0; i < argc; i++)
        printf("%d Parâmetro: %s\n", i, argv[i]);

    if (argc != 3) {
        printf("erro: sintaxe invalida!\n");
        printf("usage: ./arguments <ARG1> <ARG2>\n");
        return 1;
    }
    
    if (!valid(argv[1]) || !valid(argv[2])) {
        printf("erro: sintaxe dos argumentos inválida!\n");
        return 1;
    }

    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    printf("Resultado: %d\n", num1 * num2);
    return 0;
}
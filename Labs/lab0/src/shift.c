#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BIT(n)   (1 << (n))

int is_bit_um(int valor, int num_bit)
{
    int num_bits_int = sizeof(valor) * 8;
    assert( num_bit < num_bits_int );
    int mascara_num_bit = (1 << num_bit);
    return valor & mascara_num_bit;
}

int valid(const char *arg) {
    return atoi(arg) || (arg[0] == '0' && strlen(arg) == 1);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("erro: sintaxe invalida!\n");
        printf("usage: ./shift <VAL> <N>\n");
        return 1;
    }
    
    if (!valid(argv[1]) || !valid(argv[2])) {
        printf("erro: sintaxe dos argumentos inválida!\n");
        return 1;
    }

    int val = atoi(argv[1]);
    char n = atoi(argv[2]);

    if (n < 0 || n > 7) {
      printf("erro: valor de 'n' invalido [0..7]\n");
      return 1;
    }

    val = val | BIT(n);
    printf("O novo valor de 'val': %d\n", val);
  
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BIT(n)   (1 << (n))

int is_bit_um(int valor, int num_bit)
{
  int num_bits_int = sizeof(valor) * 8;
  assert( num_bit < num_bits_int );
  int mascara_num_bit = (1 << num_bit);
  return valor & mascara_num_bit;
}

int main(int argc, char *argv[])
{
  char val;
  int n;

  printf("Insira o valor de val: ");
  scanf("%c", &val);

  printf("Insira o valor de n: ");
  scanf("%d", &n);

  if (n < 0 || n > 7) {
    printf("error: valor de 'n' invalido [0..7]\n");
    return 1;
  }

  val = val | BIT(n);
  printf("O novo valor de 'val': 0x%8x\n", val);
  
  return 0;
}
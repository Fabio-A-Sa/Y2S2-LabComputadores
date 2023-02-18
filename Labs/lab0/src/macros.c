#include <stdio.h>
#include <stdlib.h>

#define TRUE    1
#define FALSE   0
#define BIT(N) (1 << (N))

int is_number(char c)
{
  if (c >= '0' && c <= '9')
    return TRUE;
  return FALSE;
}

int main() {
    for (int i = 0 ; i < 8 ; i++)
        printf("Bit %d is %3d in decimal\n", i, BIT(i));
    printf("00010011 is %d in decimal\n", BIT(0) | BIT(1) | BIT(4));
    return 0;
}
# 2 - The C Language

## C vs. C++

A linguagem C é uma sub conjunto da linguagem C++. Esta última permite orientação a objectos e diferentes tipos de dados nativos, bem como streams / funções próprias para inputs e outputs.

```c
#include <stdio.h>

int main () {
    printf("Hello World!\n");
    return 0;
}
```
<p align = "center">Código C para mostrar o famoso `Hello World`</p>

## Inputs e Outputs em C

```c
#include <stdio.h>

int main () {
    
    // variables
    char letter = 'F';
    unsigned int unsigned_value_hx = 0x68;
    int value = 120;
    unsigned int unsigned_value = 34324;
    long long_value = 287;
    unsigned long int unsigned_long_int_value = 2453;
    char string[] = "Hello\n";
    int* pointer = &value;

    // output using printf() method
    printf("Char: %c \n", letter);
    printf("Unsigned integer in hexadecimal: %x \n", unsigned_value_hx);
    printf("Integer in decimal: %d \n", value);
    printf("Unsigned int in decimal: %u \n", unsigned_value);
    printf("Long in decimal: %l \n", long_value);
    printf("Unsigned long in decimal: %lu \n", unsigned_long_int_value);
    printf("String: %s ", string);
    printf("Address: %p \n", pointer);

    return 0;
}
```

## Bitwise Operators

- AND (&)

```note
010011 & 011001 = 010001                -- Só é 1 quando encontra dois 1s
```

- OR ( | )

```note
010011 | 011001 = 011011                -- Só é 0 quando encontra dois 0s
```

- XOR (^)

```note
010011 ^ 011001 = 001010                -- 1 para diferentes, 0 para iguais
```

- One's Compement (~)

```note
~010011 = 101100                        -- 0 passa a 1 e 1 passa a 0
```

## Shift Operators

- Right Shift ( >> )

```note
00010011 >> 4 = 00000001                -- Adiciona zeros atrás do número, dividindo-o por 2^n
```

- Left Shift ( << )

```note
00010011 << 3 = 10011000                -- Adiciona zeros à frente do número, multiplicando-o por 2^n
```

## Para evitar erros:

1. Usar mask e macros na definição dos programas:

```c
#define BIT(n) (0x1 << (n))
#define SQR_WAVE_BIT0 1
#define SQR_WAVE_BIT1 2
int mode |= BIT(SQR_WAVE_BIT1) | BIT(SQR_WAVE_BIT0); // ativa os dois bits menos significativos
```

2. Antes de uma operação aritmética, converter tudo para inteiro usando um `cast`. Assim não há overflow. Lembrando que para n bits, o valor a representar está compreendido entre [-2^n, 2^n - 1]. <br>

```c
void showErrors() {

    // erro comum 1
    signed char cresult, c1, c2, c3;
    c1 = 100;
    c2 = 3;
    c3 = 4;
    cresult = c1 * c2 / c3; // overflow pois um char não consegue conter 400

    // erro comum 2
    uchar port = 0x54;
    uchar result_8 = ( ~port ) >> 4; // se tiver 32 bits e não 8 aparecerá FFs em vez de 00s

    // solução -> um cast antes do resultado
    cresult = ( (int) c1 * c2 ) / c3;
    uchar result_8 = (uint8_t) ( ~port ) >> 4;
}
```

3. A biblioteca <stdint.h> vai ser usada e possui vários tipos extra de inteiros para manipular:

- uint8_t
- int8_t
- uint16_t
- int16_t
- uint32_t
- int32_t
- uint64_t
- int64_t

## C Pointers


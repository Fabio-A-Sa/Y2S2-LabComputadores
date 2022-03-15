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

## Bitwise Operations

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


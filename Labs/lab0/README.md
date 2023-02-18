# Linguagem C

## Tópicos

- [Argumentos](#argumentos)
- [Máscaras](#máscaras)
- Macros
- Shifts

## Argumentos

Na linguagem de programação C é possível passar parâmetros de entrada num programa, utilizando o argc e o argv. A função main recebe dois parâmetros de entrada:

```c
#include <stdio.h>
int main(int argc, char *argv[])
{
  printf("argc: %d\n", argc);
  printf("argv: %s\n", argv[0]);
  return 0;
}
```

`argc`  – valor inteiro que indica o número de argumentos que foram passados ao chamar o programa na linha de comandos;
`argv` – array de apontadores para caracteres, que contém os argumentos, um para cada "palavra" passada na linha de comandos.

Como o argv[0] armazena sempre o nome do programa que é executado na linha de comandos, o valor do argc é no mínimo igual a 1, porque existe sempre um argumento de entrada, o nome do programa (argv[0]).

O número de argumentos passados à função é potencialmente infinito. No código seguinte enumeramos todos os argumentos enviados:

```c
int main(int argc, char *argv[])
{
  int i;
  for (i = 0; i < argc; i++)
    printf("%d Parâmetro: %s\n", i, argv[i]);
  return  0;
}
```

Convém não esquecer que os argumentos são todos tratados inicialmente como strings. Para transformá-los, quando aplicável, em inteiros podemos usar a função da Standard Library `atoi`, que retorna o valor do número inteiro ou zero em caso de erro de conversão:

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("erro: sintaxe invalida!");
    return 1;
  }
  
  int num1, num2;
  num1 = atoi(argv[1]);
  num2 = atoi(argv[2]);
  printf("resultado: %d\n", num1 * num2);
  
  return  0;
}
```

O ficheiro `arguments.c` contém um exemplo de código em C que multiplica dois números mediante os argumentos passados. É robusto quanto ao número de argumentos passados e ao tipo dos mesmos. <br>
Para usá-lo basta no Minix3 correr os seguintes comandos:

```bash
minix$ cc -Wall arguments.c -o arguments
minix$ ./arguments <ARG1> <ARG2>
```

Note-se que para resolver o problema acima descrito, a condição seguinte para capturar um erro de conversão não está completamente correcta:

```c
// entra na condição se pelo menos uma das conversões tiver dado 0 (zero)
if (!num1 || !num2) {
    printf("erro: sintaxe dos argumentos inválida!\n");
    return 1;
}
```

O problema está no facto de que pelo menos um dos argumentos pode ser mesmo 0 (zero), e nesse caso a operação de multiplicação deve ser realizada. Para contornar este caso podemos identificar se o argumento é originalmente uma string que contenha 0. Não podemos, no entanto, fazer uma comparação direta: lembro que as strings em C terminam todas com o caracter de terminação '\0' e a comparação entre strings e chars não é permitida. Uma solução para identificar se um argumento é um zero poderá ser a seguinte:

```c
// "0" é igual a "0\0", mas strlen("0") == 1
if (argv[1] == '0')    // errado
if (argv[1][0] == '0') // errado, pois "0fgh" deverá também dar erro
if (argv[1][0] == '0' && strlen(argv[1]) == 1) // certo
```

Assim, a condição final ficará:

```c
if ((!num1 && !(argv[1][0] == '0' && strlen(argv[1]) == 1)) || 
                (!num2 && !(argv[2][0] == '0' && strlen(argv[2]) == 1))) {
    printf("erro: sintaxe dos argumentos inválida!\n");
    return 1;
}
```

Ou de forma mais organizada:

```c
int valid(const char *arg) {
    return atoi(arg) || (arg[0] == '0' && strlen(arg) == 1);
}

if (!valid(argv[1]) || !valid(argv[2])) {
    printf("erro: sintaxe dos argumentos inválida!\n");
    return 1;
}
```

## Máscaras

Em linguagem C, o tamanho de dados mínimo que se pode alterar é um byte (8 bits):

```c
#include <stdio.h>

int main(int argc, char *argv[])
{
  unsigned char c;

  // definir o valor inicial de c
  c = 95;  // 0b01011111
  printf("c: %d (%c)\n", c, c);

  // alterar o valor de c
  c = 100; // 0b01100100
  printf("c: %d (%c)\n", c, c);

  return 0;
}
```

Para alterar algum bit do valor original é necessário usar máscaras e operações de *bitwise*:

### AND &

Importante para desativar um determinado bit

```note
  0011 1000
& 1010 1001
-------------
  0010 1000
```

### OR |

Importante para ativar um determinado bit

```note
  0011 1000
| 1010 1001
-------------
  1011 1001
```

### XOR ^

Importante para inverter um determinado bit

```note
  0011 1000
^ 1010 1001
-------------
  1001 0001
```

O ficheiro `masks.c` contém um exemplo de código em C que calcula valores usando estas três operações <br>
Para usá-lo basta no Minix3 correr os seguintes comandos:

```bash
minix$ cc -Wall masks.c -o masks
minix$ ./masks
```

### Curiosidade: paridade de números

Estas operações de bitwise dão, por exemplo, para determinar se um número é par ou ímpar. Na sua versão tradicional o código podia ser implementado da seguinte maneira:

```c
// Return 0 -> number is even
// Return 1 -> number is odd
int is_odd_v1(const char number) {
    return number % 2;
}
```

Utilizando a operação AND é possível determinar se o número é ímpar simplesmente assim:

```c
// Se number = 0x???????1 (ímpar), então 0x???????1 & 0x00000001 = 1
// Se number = 0x???????0 (par),   então 0x???????0 & 0x00000001 = 0
int is_odd_v2(const char number) {
    return number & 1;
}
```
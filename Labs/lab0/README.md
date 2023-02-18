# Linguagem C

## Tópicos

- [Argumentos](#argumentos)
- [Máscaras](#máscaras)
- [Macros](#macros)
- [Shifts](#shifts)

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

`argc`  – valor inteiro que indica o número de argumentos que foram passados ao chamar o programa na linha de comandos; <br>
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

## Macros

De uma forma muito simples, as macros em linguagem C são basicamente substituições de strings. As Macros são tratadas (expandidas) pelo pré-processador e criadas em linguagem C através da diretiva #define. Normalmente, as macros são definidas em letras maiúsculas, desta forma é fácil identificar uma macro ao ler o código de um programa.

```c
#define TRUE    1
#define FALSE   0

int is_number(char c)
{
  if (c >= '0' && c <= '9')
    return TRUE;
  return FALSE;
}
```

Quando o código for compilado, o pré-processador substitui cada uma das macros pelo seu valor. Neste caso, onde aparece TRUE será substituído por 1, e onde aparece FALSE, será substituído por 0. <br>
Em LCOM ao longo do semestre iremos usar macros ao implementar device drivers para o sistema operativo MINIX. A mais conhecida e importante é a BIT:

```c
// BIT(0) = 0b00000001 = 1
// BIT(1) = 0b00000010 = 2
// BIT(2) = 0b00000100 = 4
// ...
#define BIT(N) (1 << (N))
```

O ficheiro `macros.c` contém um exemplo de código em C que calcula valores usando a macro anterior. <br>
Para usá-lo basta no Minix3 correr os seguintes comandos:

```bash
minix$ cc -Wall macros.c -o macros
minix$ ./macros
```

## Shifts

Tal como o nome sugere, os operadores de shifting / deslocamento efectuam o deslocamento de bits. Na linguagem C existem dois operadores de deslocamento:
- deslocamento à esquerda: valor << n, que é constituído por dois operandos (valor e n) e um operador <<;
- deslocamento à direita: valor >> n, que é constituído por dois operandos (valor e n) e um operador >>.

Caso o deslocamento fosse de n bits para a esquerda, perder-se-iam os n bits mais significativos, sendo ainda acrescentados n bits a 0 como bits menos significativos.
 
Quando efectuada sobre a representação binária de um número inteiro, a operação deslocamento para a esquerda em n bits produz um valor que corresponde à multiplicação por 2^n do valor inteiro original. Por exemplo, o deslocamento em um bit para a esquerda do valor original 00010010 que corresponde ao inteiro 18 em base decimal, é transformado no valor 00100100 que corresponde ao valor 36 em base decimal, isto é, ao dobro do valor original.

O operador deslocamento para a direita funciona de forma análoga ao operador de deslocamento para a esquerda, alterando-se somente o sentido do deslocamento.
Assim, na operação de deslocamento à direita de n bits, há lugar à deslocação de n bits para a direita.

#### Deteção do estado do bit N

```c
int is_bit_um(int valor, int num_bit)
{
  int num_bits_int = sizeof(valor) * 8;
  assert( num_bit < 32 );
  int mascara_num_bit = (1 << num_bit);
  return valor & mascara_num_bit;
}
```

#### Ativação seletiva do bit N

```c
#define BIT(n)   (1 << (n))

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
```

O ficheiro `shift.c` contém um exemplo de código em C que calcula o deslocamento de N bits à direita de um valor val. <br>
Para usá-lo basta no Minix3 correr os seguintes comandos:

```bash
minix$ cc -Wall shift.c -o shift
minix$ ./shift
```
# 5 - Keyboard

Sempre que uma tecla do teclado é pressinada, o teclado envia um comando (**scancode**) para o KBC (*Keyboard Controller*). O código pode ser:

1. `Make code`, se a tecla for pressionada. O bit mais significativo está a 0;
2. `Break code`, se a tecla deixar de ser primida. O bit mais significativo está a 1;

Por exemplo, a tecla *ESC* tem Make Code = 0x01 e Break Code = 0x81. Assim, os códigos só diferem no MSB (ou seja, nos Most Significant Bits). O KBC coloca esse código num registo e gera uma interrupção se estiver configurado para isso no canal IRQ = 1. 

A maioria dos *scancodes* só têm um byte de comprimento, mas algumas teclas possuem 2 bytes de comprimento. Essas teclas retornam o primeiro byte do género `0xE_`. Nesse caso, a diferença encontra-se no MSB do segundo byte lido.

A maioria dos KBC, incluíndo o instalado no Minix3, possuem algum delay entre a interrupção e o tratamento dos dados. Assim, é necessário esperar algum tempo para poder desistir do ciclo, chamando para isso uma função de sleep():

```c
#include <minix/sysutil.h>
#define DELAY_US    20000
tickdelay(micros_to_ticks(DELAY_US));
```
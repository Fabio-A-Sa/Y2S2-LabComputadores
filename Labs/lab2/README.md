# i8254, the PC's Timer

## Tópicos

- [O que é o i8254](#i8254)
- ControlWord
- Visualizar as configurações atuais
- Alteração da frequência do Timer
- Interrupções
- Compilação do código
- Testagem do código implementado

### Anexos

- Modo binário vs. Modo BCD
- MSB vs. LSB
- Boas práticas de organização do código
- Boas práticas de programação em C no contexto de LCOM

## i8254

O temporizador do computador, também conhecido como i8254, é um dos mais básicos tipos de harware que podemos programar na linguagem C. Com ele cada computador tem a funcionalidade de medir um tempo preciso sem depender da rapidez do processador.

O i8254 implementa internamente três contadores, cada um com 16 bits (*uint16_t*):
- `Timer 0`, no registo 0x40; 
- `Timer 1`, no registo 0x41;
- `Timer 2`, no registo 0x42; 

O registo 0x43, conhecido como `control register`, é reservado à comunicação com o dispositivo através de *system calls*.

As duas *system calls* que vamos usar para este e outros dispositivos ao longo do semestre são as que se seguem. Os nomes foram atribuídos sob o ponto de vista do programa que usa o dispositivo:

- `sys_outb`, manda informação para o timer (como por exemplo uma nova configuração a ser seguida);
- `sys_inb`, recebe informação do timer (como por exemplo a configuração atual);

![Funcionamento do i8254](../../Images/Timer.png)

Na linguagem C as duas funções têm a seguinte estrutura:

```c
/**
 * @port - registo do timer que vai receber o comando (Ox40, 0x41, 0x42, 0x43)
 * @command - comando ou valor a escrever na porta selecionada
 */
int sys_outb(uint8_t port, uint8_t command);

/**
 * @port - porta do timer a consultar (0x40, 0x41, 0x42)
 * @value - será preenchido pelo valor lido do timer escolhido em @port
 */
int sys_inb(uint8_t port, uint32_t *value);
```

Note-se que o segundo argumento de `sys_inb` pode conter até 32 bits. No contexto de LCOM só serão necessários 8 bits, 1 byte, e essa diferença muitas vezes leva a erros desnecessários. Aconselha-se por esse motivo à implementação e utilização de uma função auxiliar que funciona como uma interface entre os dois tipos:

```c
int util_sys_inb(int port, uint8_t *value) {
  if (value == NULL) return 1;   // o apontador deve ser válido
  uint32_t val;                  // variável auxiliar de 32 bits
  int ret = sys_inb(port, &val); // val ficará com o valor lido de port
  *value = 0xFF & val;           // value ficará apenas com os primeiros 8 bits do resultado lido
  return ret;
}
```

### Cuidado

Sempre que quisermos algo do Timer (ler configurações, introduzir uma nova configuração, atualizar o contador interno) é preciso primeiro avisá-lo, escrevendo no registo de controlo (0x43) a ControlWord adequada. A leitura direta de qualquer um dos registos dos contadores (0x40, 0x41, 0x42) dá origem a erros e a valores errados. Assim, antes de qualquer operação de leitura `sys_inb()` é necessário uma escrita `sys_outb()`.

Exemplo:

Imagine-se que o comando `0b01001011`, ou `0x75` em hexadecimal, permite avisar o Timer 1 que vamos ler a sua configuração atual. O código correspondente dessa ação será:

```c
sys_outb(0x43, 0x75);
uint8_t configuration;
util_sys_inb(0x41, &configuration);
printf("A configuração atual do Timer1 é %02x\n", configuration);
```
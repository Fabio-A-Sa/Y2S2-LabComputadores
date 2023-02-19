# i8254, the PC's Timer

## Tópicos

- [O que é o i8254](#i8254)
- [Control Word](#control-word)
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

O temporizador do computador, também conhecido como i8254, é um dos mais básicos tipos de hardware que podemos programar na linguagem C. Com ele cada computador tem a funcionalidade de medir um tempo preciso sem depender da rapidez do processador.

O i8254 implementa internamente três contadores, cada um com 16 bits (*uint16_t*):
- `Timer 0`, no registo 0x40; 
- `Timer 1`, no registo 0x41;
- `Timer 2`, no registo 0x42; 

O registo 0x43, conhecido como `control register`, é reservado à comunicação com o dispositivo através de *system calls*.

Duas das *system calls* que vamos usar para este e outros dispositivos ao longo do semestre são as que se seguem. Os nomes foram atribuídos sob o ponto de vista do programa que usa o dispositivo:

- `sys_outb`, manda informação para o timer (como por exemplo uma nova configuração a ser seguida);
- `sys_inb`, recebe informação do timer (como por exemplo a configuração atual);

<p align="center">
  <img src="../../Images/Timer.png">
  <p align="center">Funcionamento do i8254</p>
</p><br>

Na linguagem C as duas funções têm a seguinte estrutura:

```c
/**
 * @port - registo do timer que vai receber o comando (Ox40, 0x41, 0x42, 0x43)
 * @command - comando ou valor a escrever na porta selecionada
 */
int sys_outb(uint8_t port, uint32_t command);

/**
 * @port - porta do timer a consultar (0x40, 0x41, 0x42)
 * @value - será preenchido pelo valor lido do timer escolhido em @port
 */
int sys_inb(uint8_t port, uint32_t *value);
```

### Erro típico #1 - Tipo dos argumentos

Note-se que o segundo argumento de `sys_inb` é um apontador para um inteiro de 32 bits. No contexto de LCOM só serão necessários 8 bits (1 byte) e essa diferença muitas vezes leva a erros desnecessários. Aconselha-se por esse motivo à implementação e utilização de uma função auxiliar que funciona como uma interface entre os dois tipos:

```c
int util_sys_inb(int port, uint8_t *value) {
  if (value == NULL) return 1;   // o apontador deve ser válido
  uint32_t val;                  // variável auxiliar de 32 bits
  int ret = sys_inb(port, &val); // val ficará com o valor lido de port
  *value = 0xFF & val;           // value ficará apenas com os primeiros 8 bits do resultado lido
  return ret;
}
```

No caso de `sys_outb` esse problema já não acontece. De facto um comando de 8 bits (uint8_t) é equivalente a um comando de 32 bits (uint32_t) com os 24 bits mais significativos a 0, o que acontece quando declaramos a variável.

### Erro típico #2 - Leituras inválidas

Sempre que quisermos algo do Timer (ler configurações, introduzir uma nova configuração, atualizar o contador interno) é preciso primeiro avisá-lo, escrevendo no registo de controlo (0x43) a ControlWord adequada. A leitura direta de qualquer um dos registos dos contadores (0x40, 0x41, 0x42) dá origem a erros e a valores errados. Assim, antes de qualquer operação de leitura `sys_inb()` é necessário uma escrita `sys_outb()`.

Exemplo:

Imagine-se que o comando `0b01001011`, ou `0x75` em hexadecimal, permite avisar o i8254 que vamos ler a configuração atual do Timer 1 (presente em `0x41`). O código correspondente dessa ação será:

```c
sys_outb(0x43, 0x75);               // avisar o i8254 pelo registo de controlo 0x43 com o comando adequado
uint8_t configuration;
util_sys_inb(0x41, &configuration); // ler a configuração diretamente do timer 1, 0x41
printf("A configuração atual do Timer1 é %02x\n", configuration);
```

## Control Word

As informações enviadas ao i8254 através do registo 0x43 são muitas vezes comandos de controlo. Cada comando de controlo, chamado de `control word`, possui apenas 8 bits e têm uma construção bastante restritiva:

### Para leitura da configuração de um Timer ou o valor do contador -> Read-Back Command

<p align="center">
  <img src="../../Images/ControlWord2.png">
  <p align="center">Construção do Read-Back Command</p>
</p>

#### Exemplo

Queremos ler a **configuração** do Timer 2. O conjunto de instruções a tomar será o seguinte:

```c
// BIT(7) e BIT(6) - Ativação da opção Read-Back, para podermos ler depois
// BIT(5) - Desativação da leitura do contador. Só queremos ler a configuração.
// BIT(3) - Como queremos ler o Timer 2, ativamos o BIT 3
uint8_t command = BIT(7) | BIT(6) | BIT(5) | BIT(3); // 11101000 = 0xE8
sys_outb(0x43, 0xE8);             
uint8_t configuration;
util_sys_inb(0x42, &configuration);
printf("A configuração atual do Timer2 é %02x\n", configuration);
```

### Para configurar o Timer -> Configuration Command

<p align="center">
  <img src="../../Images/ControlWord.png">
  <p align="center">Construção do Configuration Command.</p>
</p><br>

Em LCOM seguiremos quase sempre estas configurações:
- Counter initialization: 11, LSB followed by MSB;
- BCD: 0, queremos o contador em modo binário de 16 bits;
- Onde aplicável, X = 0, por questões de compatibilidade em versões posteriores de dispositivos Intel;
- Counting Mode 3;

Após a escrita do comando de configuração no registo de controlo, 0x43, é necessário injetar o valor inicial no contador pela porta correspondente (0x40, 0x41 ou 0x42).
Cada contador interno do i8254 possui um valor interno que é decrementado a cada ciclo de relógio, ou seja no caso do Minix, 1193182 vezes por minuto. Sempre que o valor fica a 0 o dispositivo avisa o CPU (gera uma **interrupção**, algo a estudar em breve) e volta ao valor original.

Para configurar a frequência do timer selecionado, de modo a conseguirmos por exemplo contar segundos (com uma frequência de 60Hz) através das interrupções geradas, devemos calcular o valor interno:

```c
#define TIMER_FREQUENCY 1193182
uint16_t frequency = 60;
uint16_t intern_value = TIMER_FREQUENCY / frequency;
```

Com o valor interno calculado podemos colocá-lo no timer. Como se trata tipicamente de um valor de 16 bits, então devemos separá-lo em dois valores (MSB e LSB) através de funções auxiliares e só depois enviar LSB seguido de MSB. Essas funções são definidas no ficheiro `util.c`:

```c
// LSB -> Less Significant Bits
int util_get_LSB (uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; // O apontador deve ser válido
  *lsb = 0xFF & val;         // Coloca no apontador os 8 bits menos significativos do valor
  return 0;
}

// MSB -> Most Significant Bits
int util_get_MSB (uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1; // O apontador deve ser válido
  *msb = (val >> 8) & 0xFF;  // Coloca no apontador os 8 bits mais significativos do valor
  return 0;
}
```

### Erro típico #3 - Configurações incompletas

Por segurança só devemos modificar as configurações que necessitamos mesmo, deixando os outros bits iguais aos que o Sistema Operativo decidiu. Uma forma simples de contornar a situação é consultar primeiro a configuração atual do dispositivo e só depois modificar o desejado.

#### Exemplo:

Queremos configurar o Timer 1 com frequência de 60Hz. O conjunto de instruções a tomar será o seguinte:

```c
// Consultar a configuração atual do Timer 1
uint8_t readback_command = = BIT(7) | BIT(6) | BIT(5) | BIT(2); // 11100100 = 0xE4
sys_outb(0x43, 0xE4);             
uint8_t old_configuration, new_configuration;
util_sys_inb(0x41, &old_configuration);

// Novo comando de configuração, ativamos os bits da zona 'LSB followed by MSB' e mantemos os restantes
new_configuration = old_configuration | BIT (7) | BIT(6);

// Cálculo do valor inicial do contador e partes mais e menos significativas
uint16_t initial_value = TIMER_FREQUENCY / 60;
uint8_t lsb, msb;
util_get_lsb(initial_value, &lsb);
util_get_msb(initial_value, &msb);

// Avisamos o i8254 que vamos configurar o Timer 1
sys_outb(0x43, new_configuration);

// Injetamos o valor inicial do contador (lsb seguido de msb) diretamente no registo 0x41 (Timer 1)
sys_outb(0x41, lsb);
sys_outb(0x41, msb);
```
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

O registo 0x43, conhecido como `control register`, é reservado à comunicação com o dispositivo através de *system calls*. Essas são importantes para configurar a frequência da contagem, o número atual

As duas *system calls* que vamos usar para este e outros dispositivos ao longo do semestre são as duas que se seguem. Os nomes foram atribuídos sob o ponto de vista do programa que usa o dispositivo:

- `sys_outb`, manda alguma informação para o timer (como por exemplo uma nova configuração);
- `sys_inb`, recebe alguma informação do dispositivo (lê a configuração atual do timer ou valor atual do contador);

Sob o ponto de vista da linguagem C as duas funções são as seguintes:

```c
int sys_inb();
int sys_outb();
```

Note-se que

### Leitura e Escrita no Timer

A

![Funcionamento do i8254](../../Images/Timer.png)

#### Cuidado: o timer é esquisito!

Sempre que quisermos algo do Timer (ler configurações, consultar o contador) é preciso primeiro avisá-lo, escrevendo no registo de controlo (0x43) a ControlWord adequada. A leitura direta de qualquer um dos registos dos contadores (0x40, 0x41, 0x42) dá origem a erros e a valores errados. Assim, antes de qualquer operação de leitura `sys_inb()` é necessário uma escrita `sys_outb()`.


# i8042, the PC's Mouse

## Tópicos

- [Funcionamento do Rato](#rato)
- [i8042 Mouse](#i8042-mouse)
- [Interrupções](#interrupções)
- [Máquinas de Estado em C](#maquinas-de-estado-em-c)
- [Compilação do código](#compilação-do-código)
- [Testagem do código](#testagem-do-código)

## Rato

O Sistema Operativo por padrão atribui umas coordenadas iniciais no ecrã ao cursor, por esse motivo aparece sempre na mesma posição quando ligamos o computador. Depois disso o dispositivo emite bytes descrever o valor absoluto do deslocamento no eixo X, o valor absoluto do deslocamento no eixo Y e um byte extra a indicar o sinal das duas componentes e se houve algum botão (direito ou esquerdo) pressionado no processo. Todas as seguintes posições do rato são calculadas tendo por base a soma de vetores:

<p align="center">
  <img src="../../Images/Mouse.png">
  <p align="center">Interpretação da mudança das coordenadas do cursor</p>
</p><br>

Note-se o sentido dos eixos no ecrã. De P1 para P2 houve um deslocamento positivo nos dois eixos mas de P2 para P3 o deslocamento em Y foi negativo. O deslocamento do cursor para fora do quadrante positivo dos eixos não é permitido nem possível pois as variáveis que representam as coordenadas atuais são dois inteiros sem sinal.

A resolução padrão do Mouse do Minix é 4 contagens por milímetro percorrido.

## i8042 Mouse

O rato é controlado pelo mesmo dispositivo do teclado: o i8042. Vamos portanto usar as mesmas funções para ler, escrever e consultar o status do controlador:

```c
int read_KBC_status(uint8_t *status);
int write_KBC_command(uint8_t port, uint8_t commandByte);
int read_KBC_output(uint8_t port, uint8_t *output);
```

// soon

## Interrupções

O rato está presente na IRQ_LINE 12. As funções de subscrição e desistência das interrupções são muito semelhantes às anteriores. De igual forma temos que declarar as interrupões como exclusivas:

```c
/* ------ i8042.h ------ */
#define MOUSE_IRQ 12;   

/* ------ mouse.c ------ */
int mouse_hook_id = 2;

// subscribe interrupts
int mouse_subscribe_int (uint8_t *bit_no) {
  if(bit_no == NULL) return 1;   // o apontador tem de ser válido
  *bit_no = BIT(mouse_hook_id);  // a função que chamou esta deve saber qual é a máscara a utilizar
                                 // para detectar as interrupções geradas
  // subscrição das interrupções em modo exclusivo
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

// unsubscribe interrupts
int keyboard_unsubscribe_int () {
  return sys_irqrmpolicy(&timer_hook_id); // desligar as interrupções
}
```

## Máquinas de Estado em C

// soon

## Compilação do código

// soon

## Testagem do código

// soon

---

@ Fábio Sá <br>
@ Fevereiro de 2023
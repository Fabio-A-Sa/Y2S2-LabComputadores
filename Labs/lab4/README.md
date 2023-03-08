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

// soon

## i8042 Mouse

O rato é controlado pelo mesmo dispositivo do teclado: o i8042. Vamos portanto usar as mesmas funções para ler, escrever e consultar o status do controlador:

```c
int read_KBC_status(uint8_t *status);
int write_KBC_command(uint8_t port, uint8_t commandByte);
int read_KBC_output(uint8_t port, uint8_t *output);
```

A estrutura do código será semelhante ao Lab anterior:

<p align="center">
  <img src="../../Images/Code.png">
  <p align="center">Organização do código a implementar</p>
</p><br>

// soon

## Interrupções

O rato está presente na IRQ_LINE 12. As funções das interrupções são muito semelhantes às anteriores. De igual forma temos que declarar as interrupões como exclusivas:

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
int mouse_unsubscribe_int () {
  return sys_irqrmpolicy(&mouse_hook_id); // desligar as interrupções
}
```

## Máquinas de Estado em C

A gestão das interrupções geradas pelos dispositivos estudados até aqui pode constituir um modo de `Event Driven Design`. Nesse caso o fluxo do programa é controlado pelo ambiente onde está inserido, ou seja, é reativo na resposta aos eventos (interrupções) que poderão ocorrer de forma assíncrona. No entanto, para o contexto do Projeto de LCOM este *design* de código não é suficiente para garantirmos um código robusto, modular e facilmente manipulável. A função do Lab4 `mouse_test_gesture(uint8_t x_len, uint8_t tolerance)` é um exemplo bom e complexo para explorar.

A ideia é desenhar um símbolo AND (V invertido) com o rato garantindo várias restrições:
- A primeira linha deve ser desenhada SÓ com o botão esquerdo pressionado;
- A segunda linha deve ser desenhada SÓ com o botão direito pressionado;
- No vértice o botão esquerdo tem de deixar de ser pressionado antes do botão direito ser;
- O valor absoluto da inclinação em cada linha deve ser maior que 1;
- O deslocamento em X não pode ser inferior a x_len; 
- Há uma tolerância de algumas unidades, tolerance, em cada iteração para cada eixo;
- O fim de cada linha é marcado pela libertação do botão;
- Um erro na execução faz com que o sistema volte ao estado inicial;

Este sistema tem demasiadas restrições para ser implementado com base em variáveis booleanas e cadeias de condições if-else. No entantanto podemos pensar no mesmo como um conjunto de estados:

- Estado inicial, Start (S)
- Linha ascendente, Up (U)
- Vértice, Vertex (V)
- Linha descendente, Down (D)
- Estado final, End (E)

A transição entre um estado e outro ocorre depois de cumprida uma determinada condição. Caso essa condição não seja cumprida o sistema ou permanece no mesmo estado ou deverá retornar ao estado inicial. De acordo com a matéria lecionada em Teoria da Computação o enunciado da função pode ser representado pela seguinte Máquina de Estados:

<p align="center">
  <img src="../../Images/StateMachine.png">
  <p align="center">Máquina de Estados que representa o fluxo do programa</p>
</p><br>

Descrição das transições:

- I: botão esquerdo é pressionado
- II: o botão esquerdo está pressionado enquanto o movimento do rato é ascendente com declive superior a 1;
- III: o botão esquerdo deixa de estar pressionado
- IV: o botão direito é pressionado
- V: o botão esquerdo está pressionado enquanto o movimento do rato é descendente com declive inferior a -1;
- VI: o botão esquerdo deixa de estar pressionado e o deslocamento no eixo X foi superior a x_len;
- F: corresponde à transição ELSE. Caso a condição atual não corresponda a uma manutenção do estado atual (*self-transition*) ou avanço para o seguinte estado, então o sistema deve voltar ao inicial.

Em C um conjunto de estados pode ser programado usando uma enumeração:

```c
typedef enum {
  START,
  UP,
  VERTEX,
  DOWN,
  END
} SystemState;
```

// soon

## Compilação do código

Ao longo do Lab4 programamos em 2 ficheiros:

- `mouse.c`, para implementação das funções referentes a interrupções, sincronização de pacotes e comandos enviados diretamente ao rato;
- `lab4.c`, para implementação das funções de mais alto nível que usam as funções disponíveis no módulo do *mouse*;

Ainda importamos os ficheiros `utils.c`, `timer.c`, `i8254.h`, `i8042.h` e `KBC.c` do lab anterior. Em LCOM o processo de compilação é simples pois existe sempre um makefile que auxilia na tarefa. Para compilar basta correr os seguintes comandos:

```bash
minix$ make clean # apaga os binários temporários
minix$ make       # compila o programa
```

## Testagem do código

A biblioteca LCF (*LCOM Framework*) disponível nesta versão do Minix3 tem um conjunto de testes para cada função a implementar em `lab4.c`. Assim é simples verificar se o programa corre como esperado para depois ser usado sem problemas no projeto. Para saber o conjunto dos testes disponíveis basta consultar:

```bash
minix$ lcom_run lab4
```

Neste caso em concreto estão disponíveis algumas combinações:

```bash
minix$ lcom_run lab4 "packet <NUMBER_PACKETS> -t <0,1,2,3,4,5>"
minix$ lcom_run lab4 "async <TIME_SECONDS> -t <0,1,2,3,4,5>"
minix$ lcom_run lab4 "remote <TIME_MILLISECONDS> -t <0,1,2,3,4,5>"
minix$ lcom_run lab4 "gesture <X_LENGTH> <TOLERANCE> -t <0,1,2,3,4,5>"
```

---

@ Fábio Sá <br>
@ Março de 2023
# Template

Este template reune aspetos relevantes para o projeto final de LCOM, como dicas, algoritmos, orientação na estruturação do código, otimizações possíveis entre outros.

## Tópicos

- [Setup](#setup)
- [Funcionalidades implementadas](#funcionalidades-implementadas)
- [Estrutura MVC](#estrutura-mvc)
- [Orientação a Objetos em C](#orientação-a-objetos-em-c)
- [Máquinas de Estado em C](#maquinas-de-estado-em-c)
- XPM em modo direto
    - Construção
    - Utilização
- Otimizações
    - Flags de compilação
    - Double Buffering
    - Operações frequentes
- Debug

## Setup

O template pode ser executado com as seguintes instruções:

```bash
$ git clone https://github.com/Fabio-A-Sa/Y2S2-LabComputadores.git
$ cd Y2S2-LabComputadores/Template
$ make clean && make
$ lcom_run proj
```

O programa depende das configurações colocadas em `config.h`. É possível mudar a frequências tamanhos de ecrã, modos de cores e verificar a importância da otimização [double buffering](#double-buffering).

```c
#define GAME_FREQUENCY      60     // 30
#define VIDEO_MODE          0x115  // 0x14C
#define DOUBLE_BUFFER       1      // 0
```

Com a variação destes parâmetros conclui-se que:
- Uma frequência superior a 60 Hz não apresenta ganhos significativos;
- Modos superiores a 0x115 podem ser lentos (maior memória manipulada, mais bytes por pixel, maior resolução);
- A aplicação implementada com **double buffering** é substancialmente mais fluída;

## Funcionalidades implementadas

Apesar do programa não fazer nada de útil, apresenta praticamente todos os truques necessários para realizar o projeto: gestos do cursor, cliques, lidar com diferentes objetos, diferentes menus e interação entre os diversos dispositivos.

O template tem três menus:
- A tecla `S` acede ao menu start;
- A tecla `G` acede ao menu game;
- A tecla `E` acede ao menu end;

Em qualquer momento, a tecla `Q` permite terminar a execução. Em todos os menus o cursor está presente e funcional.

<p align="center">
  <img src="../Images/Template1.png" width=300 height=250>
  <p align="center">Menu G</p>
</p><br>

No menu `G` é possível clicar num de quatro botões, alterando a cor do quadrante respetivo. O cursor nesta etapa tem outro formato além do tradicional.

## Estrutura MVC

Baseado nos [slides](https://web.fe.up.pt/~arestivo/slides/?s=patterns#58) do André Restivo.

<p align="center">
  <img src="../Images/TemplateMVC.png">
  <p align="center">MVC</p>
</p><br>

#### Model

O modelo conhece os objetos do programa, como Botões, o Cursor, Sprites, o estado do Menu, o estado do Sistema. As interrupções tratadas no controlador afetam diretamente o modelo e as interações entre os objetos através de [máquinas de estado](#máquinas-de-estado-em-c).

#### Controller

Contém todos os controladores dos dispositivos implementados durante as aulas práticas. Todas as funções de alto nível, nomeadamente as presentes em ficheiros labX.c, foram eliminadas. De igual forma partes do código foram modificadas para uma melhor modularidade. Por exemplo:
- Agora a [subscrição das interrupções](./controller/timer/timer.h) é realizadas sem passagem da máscara como argumento;
- A estrura mouse_info_t foi substituida por [MouseInfo](./controller/mouse/mouse.h), que é mais compacta e adaptada às necessidades da aplicação; 
- [VBE.h](./controller/video/VBE.h) agora também contém as cores manipuladas pela VRAM;

#### View

Módulo que trata da representação visual do modelo sempre que há uma interrupção por parte do timer.

## Orientação a Objetos em C

O objeto Sprite, usado para criar e manipular os elementos do cenário, é na realidade uma struct:

```c
typedef struct {
    uint16_t height;    // altura, em pixeis
    uint16_t width;     // largura, em pixeis
    uint32_t *colors;   // array de cores, se for criado a partir de um XPM
    uint32_t color;     // cor única, se for um rectângulo simples como o botão
    uint8_t pressed;    // indica se o objeto foi pressionado pelo rato
} Sprite; 
```

Por motivos de eficiência nem sempre são criados com XPM. Por exemplo, no caso do botões que têm cor única, é aconselhável usar apenas o parâmetro *color* e não preencher o array *colors*:

```c
#include "xpm/mouse.xpm"

Sprite *mouse;
Sprite *button1;

void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    button1 = create_sprite_button(mode_info.XResolution/2, mode_info.YResolution/2, ORANGE);
}

Sprite *create_sprite_xpm(xpm_map_t sprite);
Sprite *create_sprite_button(uint16_t width, uint16_t height, uint32_t color);
```

Ao lidar com apontadores e alocação dinâmica de memória é possível tratar a estrutura como se fosse um objeto em C++:

```c
void update_buttons_state() {
    if (mouse_info.left_click) {
        if (mouse_info.x < mode_info.XResolution/2 && mouse_info.y < mode_info.YResolution/2)
            button1->pressed = 1;
        //...
    }
}
```

É boa prática antes de acabar o programa libertar a memória alocada:

```c
void destroy_sprite(Sprite *sprite) {
    if (sprite == NULL) return;
    if (sprite->colors) free(sprite->colors);
    free(sprite);
    sprite = NULL;
}
```

## Máquinas de Estado em C

A gestão das interrupções geradas pelos dispositivos estudados até aqui pode constituir um modo de `Event Driven Design`. Nesse caso o fluxo do programa é controlado pelo ambiente onde está inserido, ou seja, é reativo na resposta aos eventos (interrupções) que poderão ocorrer de forma assíncrona. No entanto, para o contexto do Projeto de LCOM este *design* de código não é suficiente para garantirmos um código robusto, modular e facilmente manipulável. [Aqui](../Labs/lab4/README.md#máquinas-de-estado-em-c) há um exemplo mais complexo do que o apresentado de seguida.

A transição entre menus é semelhante a transições numa máquina de estados:

<p align="center">
  <img src="../Images/TemplateMachine.png">
  <p align="center">Máquina de Estados para Menus</p>
</p><br>

Em C um conjunto de estados pode ser programado usando uma enumeração e as transições com recurso a switch-case:

```c
typedef enum {
    RUNNING,
    EXIT,
} SystemState;

typedef enum {
    START,
    GAME,
    END
} MenuState;

// condições iniciais
SystemState systemState = RUNNING;
MenuState menuState = START;

// a máquina de estados é atualizada de acordo com 
// o valor do scancode lido em cada interrupção
void update_keyboard_state() {
    (kbc_ih)();
    switch (scancode) {
        case Q_KEY:
            systemState = EXIT;
            break;
        case S_KEY:
            menuState = START;
            break;
        case G_KEY:
            menuState = GAME;
            break;
        case E_KEY:
            menuState = END;
        default:
            break;
    }
}
```

A atualização do estado tem consequências imediatas. Do lado do módulo View, o estado do Model é consultado e a partir daí é escolhido o menu:

```c
extern MenuState menuState;

void draw_new_frame() {
    switch (menuState) {
        case START:
            draw_initial_menu();
            break;
        case GAME:
            draw_game_menu();
            break;
        case END:
            draw_finish_menu();
            break;
    }
    draw_mouse();
}
```

## XPM em modo direto

### Construção

### Utilização

## Otimizações

### Flags de compilação

### Double Buffering

### Operações frequentes

## Debug



---

@ Fábio Sá <br>
@ Abril de 2023
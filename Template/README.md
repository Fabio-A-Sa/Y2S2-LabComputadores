# Template

Este template reune aspetos relevantes para o projeto final de LCOM, como dicas, algoritmos, orientação na estruturação do código, otimizações possíveis entre outros.

## Tópicos

- [Setup](#setup)
- [Funcionalidades implementadas](#funcionalidades-implementadas)
- [Estrutura MVC](#estrutura-mvc)
- Orientação a Objetos em C
- Máquinas de Estado em C
- XPM em modo direto
    - Construção
    - Utilização
- Otimizações
    - Flags de compilação
    - Double Buffering
    - Operações frequentes

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

#### Model

#### Controller

#### View

Contém todos os controladores dos dispositivos.

Baseado nos [slides](https://web.fe.up.pt/~arestivo/slides/?s=patterns#58) do André Restivo:



---

@ Fábio Sá <br>
@ Abril de 2023
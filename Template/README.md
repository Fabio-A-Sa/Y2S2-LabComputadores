# Template

Este template reune aspetos relevantes para o projeto final de LCOM, como dicas, algoritmos, orientação na estruturação do código, otimizações possíveis entre outros.

## Tópicos

- [Setup](#setup)
- [Funcionalidades implementadas](#funcionalidades-implementadas)
- [Estrutura MVC](#estrutura-mvc)
- XPM em modo direto
    - Construção
    - Utilização
- Otimizações
    - Flags de compilação
    - Double Buffering
    - Operações frequentes
- Orientação a Objetos em C
- Máquinas de Estado em C

## Setup

O template pode ser executado com as seguintes instruções:

```bash
$ git clone https://github.com/Fabio-A-Sa/Y2S2-LabComputadores.git
$ cd Y2S2-LabComputadores/Template
$ make clean && make
$ lcom_run proj
```

O modo de execução depende das configurações colocadas em `config.h`. É possível executar o programa com várias frequências, vários tamanhos de ecrã, vários modos de cores e verificar a importância da otimização [double buffering](#double-buffering).

```c
#define GAME_FREQUENCY      60     // 30
#define VIDEO_MODE          0x115  // 0x14C
#define DOUBLE_BUFFER       1      // 0
```

- Uma frequência superior a 60 Hz não apresenta ganhos significativos;
- Modos superiores a 0x115 podem demorar (maior memória manipulada)
- A aplicação implementada com **double buffering** é substancialmente mais

## Funcionalidades implementadas

## Estrutura MVC

Baseado nos [slides](https://web.fe.up.pt/~arestivo/slides/?s=patterns#58) do André Restivo:



---

@ Fábio Sá <br>
@ Abril de 2023
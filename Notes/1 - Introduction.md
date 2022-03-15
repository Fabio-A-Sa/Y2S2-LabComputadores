# 1 - Introduction

## Programa vs Processo

### Programa

Pedaço de código com um conjunto de instruções que pode ser executado pelo processador.

### Processo

Abstração do sistema operativo de um programa em execução.

## Utilização da memória

A memória é usada em diversas partes para assegurar o bom funcionamento dos programas. Há 5 partes principais:

<p align = "center" >
<img    align = "center"
        src = "..//Images//MemoryParts.png"
        alt = "memory parts"
        title = "memory parts"
/> </p>

1. args - argumentos passados pela linha de comando e variáveis de ambiente;
2. stack - registos de ativação para funções invocadas;
3. heap - região alocada dinamicamente usando `malloc` em C;
4. data - região de memória alocada estaticamente pelo compilador, por exemplo com strings, variáveis e afins presentes no código;
5. text - instruções do programa;

### As 3 camadas estruturais do computador:

- Aplicações (de sistema) e programas;
- Sistema operativo;
- Hardware;

O Kernel implementa as chamadas ao sistema e manipula o hardware. É diferente de sistema operativo, pois este último ainda engloba as livrarias, as aplicações e programas de sistema.

## I/O Devices

Providenciam uma ligação / interface entre o CPU e o mundo externo (o hardware, por exemplo). São controlados por um controlador ou adaptador, que pode ser manipulado usando linguagens como Assembly ou por mapeamento de registos de memória. <br>
Têm a seguinte estrutura:

1. Control - pede operações de input ou output;
2. Status - o estado do dispositivo pendente ou operações pendentes;
3. Data - que vai ser transferida e/ou obtida pelo dispositivo;
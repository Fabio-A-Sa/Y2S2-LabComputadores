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
3. heap - região alocada dinamicamente usando `malloc`em C;
4. data - região de memória alocada estaticamente pelo compilador, por exemplo com strings, variáveis e afins presentes no código;
5. text - instruções do programa;

### As 3 camadas estruturais do computador:

- Aplicações (de sistema) e programas;
- Sistema operativo;
- Hardware;


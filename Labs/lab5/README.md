# Video Card

## Tópicos

- [Video Modes](#video-modes)
- [Mapeamento da Video RAM](#mapeamento-da-video-ram)
- [Sprites](#sprites)
- [Compilação do código](#compilação-do-código)
- [Testagem do código](#testagem-do-código)

## Video Modes

O Minix possui dois modos de funcionamento:
- `Text Mode`, o modo padrão do terminal que vimos até agora;
- `Graphic Mode`, o que vamos explorar neste Lab5. Existem alguns modos gráficos que permitem variar o ecrã ao nível da dimensão (resolução vertical e horizontal), no tipo de cores disponíveis e de como estas estão mapeadas na memória;

<p align="center">
  <img src="../../Images/VideoModes.png">
  <p align="center">Modos do Minix</p>
</p><br>

Para utilizar o modo gráfico é necessário configurá-lo através da Video Bios Extension (VBE). Este método usa duas novas ferramentas:
- uma nova *system call*, a `sys_int86`, para injetar na BIOS o comando pretendido em kernel mode;
- uma estrutura `reg86_t` que suporta um conjunto de instruções que serão passadas como argumento da função anterior;

A estrutura reg86_t possui alguns parâmetros de 16 bits cujo valor depende da configuração:

<p align="center">

| Parâmetro reg86_t | Text Mode | Graphic Mode       |
|-------------------|-----------|--------------------|
| ah                | 0x00      | 0x4F               |
| al                | Ox03      | 0x02               |
| ax                | 0x0003    | 0x4F02             |
| bx                | 0x0000    | submode \| BIT(14) |
| intno             | 0x10      | 0x10               |

</p>

AX pode ser interpretado como dois parâmetros de 8 bits: o AH (*higher bits*) e AL (*lower bits*), sendo AL responsável por determinar o modo de funcionamento (0x02 ou 0x03). BX representa o submodo de funcionamento no modo gráfico. BX também possui o BIT 14 ativo para tornar o mapeamento da memória `linear`. O parâmetro "intno" é 0x10 nos dois casos.

Dado um submodo de funcionamento, a mudança para o respectivo modo gráfico pode ser realizada recorrendo à seguinte implementação:

```c
int set_graphic_mode(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); // inicialização da estrutura com o valor 0 em todos os parâmetros
    reg86.intno = 0x10;               // intno é sempre 0x10      
    reg86.ah = 0x4F;                  // parte mais significativa de AX
    reg86.al = 0x02;                  // parte menos significativa de AX. 0x02 no caso de modo gráfico
    // reg86.ax = 0x4F02;             // equivamente às duas últimas instruções
    reg86.bx = submode | BIT(14);     // determinação do submodo com memória linear
    if (sys_int86(&reg86) != 0) {     // se houver algum erro, abortar a função
        printf("Set graphic mode failed\n");
        return 1;
    }
    return 0;
}
```

Tal como nos labs anteriores é importante deixar o Minix no mesmo estado por motivos de integridade do sistema. Assim é necessário voltar ao modo de texto antes de retornar qualquer função que use o modo gráfico. Uma possível solução:

```c
int set_text_mode() {
    reg86_t reg86;                       
    memset(&reg86, 0, sizeof(reg86));   // inicialização da estrutura com o valor 0 em todos os parâmetros
    reg86.intno = 0x10;                 // intno é sempre 0x10 
    reg86.ah = 0x00;                    // parte mais significativa de AX 
    reg86.al = 0x03;                    // parte menos significativa de AX. 0x03 no caso de modo texto
    // reg86.ax = 0x0003;               // equivamente às duas últimas instruções
    reg86.bx = 0x0000;                  // não há submodo no modo de texto
    if(sys_int86(&reg86) != 0) {        // se houver algum erro, abortar a função
        printf("Set text mode failed\n");
        return 1;
    }
    return 0;
}
```

## Mapeamento da Video RAM

A VRAM (*Video RAM*) é a memória que contém informação sobre a cor de cada píxel presente no ecrã. Antes de mudar o Minix para o modo gráfico é importante alocar memória suficiente de acordo com o submodo escolhido. Ao conjunto de memória alocada dá-se o nome de `frame buffer`.

<p align="center">
  <img src="../../Images/FrameBuffer.png">
  <p align="center">Frame Buffer</p>
</p><br>

A VRAM no modo gráfico é sempre `linear` e contínua, ou seja, a matriz de cores é na realidade um array contínuo de bytes. O seu tamanho depende:
- da resolução horizontal do modo;
- da resolução vertical do modo;
. do modelo de cores escolhido;
- do número de bytes que representam a cor de cada píxel;

| Mode  | Screen Resolution | Color Model  | Bits per pixel (R:G:B) |
|-------|-------------------|--------------|------------------------|
| 0x105 | 1024x768          | Indexed      | 8                      |
| 0x110 | 640x480           | Direct color | 15((1:)5:5:5)          |
| 0x115 | 800x600           | Direct color | 24 (8:8:8)             |
| 0x11A | 1280x1024         | Direct color | 16 (5:6:5)             |
| 0x14C | 1152x864          | Direct color | 32 ((8:)8:8:8)         |

Conhecendo HRES (resolução horizontal) é possível encontrar o índice do píxel que corresponde às coordenadas 2D do frame buffer:

```c
int pixel_index(uint16_t x, uint16_t y) {
    return y * HRES + x;
}
```

As cores podem vir em dois formatos:
- `Modelo direto`: a cor é um conjunto de bytes que representam cada uma das componentes RGB (red, green, blue) do píxel.
- `Modelo indexado`: a cor é previamente mapeada numa paleta de cores do Minix e apenas é necessário o índice dessa cor;

<p align="center">
  <img src="../../Images/Paleta.png">
  <p align="center">Paleta de cores indexadas do Minix</p>
</p><br>

Nesta versão do Minix estão disponíveis 64 cores indexadas. Sabendo as características de cada modo é simples calcular o número de bytes de memória a alocar. Por exemplo:

```c
unsigned int frame_0x105_bytes = 1024 x 768  x 1; // 8 bits = 1 byte
unsigned int frame_0x11A_bytes = 1280 x 1024 x 2; // 16 bits = 2 bytes
```

## Sprites

// soon

## Compilação do código

// soon

## Testagem do código

// soon

---

@ Fábio Sá <br>
@ Março de 2023
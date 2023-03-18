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

// soon

## Sprites

// soon

## Compilação do código

// soon

## Testagem do código

// soon

---

@ Fábio Sá <br>
@ Março de 2023
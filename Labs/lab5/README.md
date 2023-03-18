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

Para utilizar o modo gráfico é necessário configurá-lo através da Video Bios Extension (VBE). Este método usa duas novas ferramentas:
- uma nova *system call*, a `sys_int86`, para injetar na BIOS o comando pretendido em kernel mode;
- uma estrutura `reg86_t` que suporta um conjunto de instruções que serão passadas como argumento da função anterior;

A estrutura reg86_t possui alguns parâmetros de 16 bits cujo valor depende da configuração:

| Parâmetro reg86_t | Text Mode | Graphic Mode    |
|-------------------|-----------|-----------------|
| ah                | 0x00      | 0x4F            |
| al                | Ox03      | 0x02            |
| ax                | 0x0003    | 0x4F02          |
| bx                | 0x0000    | mode \| BIT(14) |
| intno             | 0x10      | 0x10            |

AX pode ser interpretado como dois parâmetros de 8 bits: o AH (*higher bits*) e AL (*lower bits*). BX em modo texto não possui um sub-modo, mas no modo gráfico. Em LCOM

```c
int vg_exit() {
    reg86_t reg86;
    reg86.intno = 0x10;
    reg86.ah = 0x00;
    reg86.al = 0x03;
    if(sys_int86(&reg86) != 0) {
        printf("vg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}
```

// soon

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
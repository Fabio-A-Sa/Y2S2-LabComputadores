# Dúvidas LCOM@2023 20/06/2023

Ficheiro resultante de uma aula de dúvidas antes do exame de recurso/melhoria.

## PP1

### 1.1 - Control Word

Um dispositivo X precisa de ser configurado de acordo com o modo dado em argumento. O `control register` é **0x123** e a `control word` tem o seguinte formato: 

- Modo 0 - BIT 3, faz X 
- Modo 1 - BIT 4, faz Y
- Modo 2 - BIT 2, faz Z
- Modo 3 - BIT 0, faz W

```c
#define BIT(N) (1 << (N))

int set_mode(uint8_t mode) {

    if (mode > 3) return 1;

    uint8_t control_word; // 0
    if (mode == 0) control_word = BIT(3);
    if (mode == 1) control_word = BIT(4);
    if (mode == 2) control_word = BIT(2);
    if (mode == 3) control_word = BIT(0);

    if (sys_outb(0x123, control_word) != 0) return 1;

    return 0;
}
```

### 1.2 - Interrupções

Um dispositivo X gera interrupções na IRQ LINE 7.

```c
int hook_id = 0; // [0..7]
#define IRQ_LINE 7

int enable_interrupts(uint8_t *mask) {
    if (mask == NULL) return 1;
    *mask = BIT(hook_id);
    if (sys_irqsetpolicy(IRQ_LINE, IRQ_REENABLE, &hook_id) != 0) return 1;
    return 0;
}

int disable_interrupts() {
    if (sys_irqrmpolicy(&hook_id)) return 1;
    return 0;
}
```

### 1.3 - All

O dispositivo X funciona com interrupções a 30Hz, deve ser configurado com o modo @mode e deve chamar a função pp_test_call() depois de @delay segundos.

```c
int pp1_test(uint8_t mode, uint16_t delay) {
    
    if (set_mode(mode) != 0) return 1;

    uint8_t mask;
    if (enable_interrupts(&mask) != 0) return 1;
    
    int counter = 0;
    int ipc_status;
    message msg;

    while(counter != 30 * delay) { 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { 
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 
            if (msg.m_notify.interrupts & mask) {
                counter++;
            }
            break;
            default:
            break; 
        }
        } else {
        }
    }

    pp_test_call();

    if (disable_interrupts() != 0) return 1;

    return 0;
}
```

## PP2

Temos um dispositivo gráfico X que tem os seguintes modos de funcionamento:

- Modo 0 - Modo Texto
- Modo 1 - Modo Gráfico 1, modo de cores 8
- Modo 2 - Modo Gráfico 2, modo de cores 8:8:8
- Modo 3 - Modo Gráfico 3, modo de cores 5:6:5

Temos acesso às seguintes funções do MINIX:

```c
// consulta as informações do modo @mode
int lpv_get_mode_info(uint16_t mode, lpv_mode_info_t *lpv)

// muda o modo do minix para @mode 
int lpv_set_mode(uint16_t mode)

// aloca memória virtual e física para o frame buffer
uint8_t *video_map_phys(uint32_t ph_addr, size_t len);
```

### 2.1 - Validação do Modo

```c
if (mode > 3) return 1;
```

### 2.2 -  Mudança do Modo

```c
if (lvp_set_mode(mode) != 0) return 1;
```

### 2.3 - Configurar o frame buffer

```c
lpv_mode_info_t info;
if (lpv_get_mode_info(mode, &info) != 0) return 1;

uint8_t bytes_per_pixel = (info.bpp + 7) / 8;
size_t len = info.x_res * info.y_res * bytes_per_pixel;

uint8_t *frame_buffer = video_map_phys(info.ph_addr, len);
if (frame_buffer == NULL) return 1;
```

### 2.3 - Pintar um pixel

```c
int draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    int offset = ((info.x_res * y) + x) *  bytes_per_pixel;
    // memcpy(&to, &from, qtd) | minix$ man 3 memcpy 
    if (memcpy(&frame_buffer[offset], &color, bytes_per_pixel) == NULL) return 1;
    return 0;
}
```

### 2.4 - Desenhar um quadrado

É suposto desenhar sempre a parte visível do quadrado.

```c
int draw_square(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

    for (int i = 0 ; i < len ; i++) {
        for (int j = 0 ; j < len ; j++) {

            // se sair, ignoram
            if (x+i >= info.x_res || y+j >= info.y_res) continue;

            if (draw_pixel(x + i, y + j, color) != 0) return 1;
        }
    }
}
```

## PPX

### 3.1 - Estrutura

```c
<includes>

<defines>
#DEFINE BIT(N) (1 << (N))

<variáveis globais>

int main() {
    // deixar igual ao que está lá
}

<funções auxiliares>

int pp_test_part_1(...args) {
    //TODO
}

int pp_test_part_2(...args) {
    //TODO
}
```
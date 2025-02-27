// Resolução PP3 2020 Recurso - The LCV1
// Fábio Sá 2023

#include <stdint.h>
#include <stdlib.h>

#define BIT(N) (1 << (N))
#define LCV1_PORT 0x24       // control register
#define LCV1_MASK BIT(7);    // o bit ativado durante as interrupções é o 7

uint8_t *frame_buffer;      // apontador para o início do frame buffer
size_t frame_buffer_size;   // tamanho do frame buffer
uint8_t bytes_per_pixel;    // número de bytes por píxel do modo escolhido
lpv_mode_info_t mode_info;  // reune x_res, y_res, bpp e phys_addr

int hook_id = 0;    // [0..7]
#define IRQ_LINE 7; // interrupções

int pp_enable_interrupts() {
    return sys_irqsetpolicy(IRQ_LINE, IRQ_REENABLE, &hook_id);
}

int pp_disable_interrupts() {
    return sys_irqrmpolicy(&hook_id);
}

// Configura um modo específico para o dispositivo com ou sem interrupções
int pp_set_mode(uint8_t mode, uint8_t interrupts) {
    uint8_t controlWord = BIT(mode);
    if (interrupts) controlWord |= BIT(7);
    return sys_outb(LCV1_PORT, controlWord);
}

 // Configura o frame buffer de acordo com o modo escolhido
int pp_set_frame(uint8_t mode) {
    if (lpv_get_mode_info(mode, &mode_info) != 0) return 1;
    bytes_per_pixel = (mode_info.bpp + 7) / 8; // atenção: arredondamento por excesso.
    frame_buffer_size = mode_info.x_res * mode_info.y_res * bytes_per_pixel;
    frame_buffer = video_map_phys(mode_info.ph_addr, frame_buffer_size);
    if (frame_buffer == NULL) return 1; // atenção: a video_map_phys pode retornar erro
}

// Pinta o píxel de coordenadas (x,y) com a cor dada como argumento
int pp_draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    // Cálculo do offset. Tem de multiplicar sempre pelo número de bytes por píxel
    int offset = ((y * mode_info.x_res) + x) * bytes_per_pixel; 
    // args: (&to, &from, qtd). Qualquer dúvida usar o comando "minix$ man 3 memcpy"
    if (memcpy(&frame_buffer[offset], &color, bytes_per_pixel) == NULL) return 1;
    return 0;
}

// Pinta um quadrado com canto superior esquerdo em (x,y) com lado (len) e cor (color)
// Não precisamos de normalizar a cor porque os modos são simples (8 e 8:8:8), ou seja,
// cada cor tem os seus bytes completamente preenchidos (ao contrário do modo 5:6:5, por exemplo)
int pp_draw_square(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0 ; i < len ; i++) {
        for (int j = 0 ; j < len ; j++) {

            // Ignorar quando sair fora do ecrã, apenas porque o enunciado diz isso
            if (x + i >= mode_info.x_res || y + j >= mode_info.y_res) continue;

            // Se estiver dentro do ecrã, tenta desenhar
            if (pp_draw_pixel(x + i, y + j, color) != 0) return 1;
        }
    }
}

// Parte 1: desenha um quadrado, sem interrupções
int pp_test_square_part_1(uint8_t mode, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay) {

    // Verificar inputs. O modo nunca será negativo (uintX_t) nem superior a 3.
    if (mode > 3) return 1;

    // Configuração do Video Card
    // Nesta primeira função não há interrupções. A control word é só calculada em função do modo escolhido.
    if (pp_set_mode(mode, 0) != 0) return 1;

    // Configurar o frame buffer
    if (pp_set_frame(mode)) return 1;

    // Desenhar um quadrado
    if (pp_draw_square(x, y, len, color) != 0) return 1;

    // Espera "delay" segundos até voltar ao modo texto
    sleep(delay);

    // Voltar ao modo texto (modo 0). Sem interrupções.
    if (pp_set_mode(0, 0) != 0) return 1;

    return 0;
}

// Desenha um quadrado, com interrupções
int pp_test_square_part_2(uint8_t mode, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay) {

    // Verificar inputs. O modo nunca será negativo (uintX_t) nem superior a 3.
    if (mode > 3) return 1;

    // Configuração do Video Card, agora há interrupções.
    if (pp_set_mode(mode, 1) != 0) return 1;

    // Configurar o frame buffer
    if (pp_set_frame(mode)) return 1;
    
    // Etapa principal
    // 1 - Desenha o quadrado em (x, y)
    // 2 - Espera delay segundos
    // 3 - Desenha o quadrado em (y, x)
    // 4 - Espera delay segundos
    // Como o dispositivo funciona a 30Hz, então recebemos 30 interrupções por segundo
    // Ou seja, para esperar delay segundos temos de atingir (30 * delay) interrupções

    // O ponto 1 pode ser feito fora do loop
    if (pp_draw_square(x, y, len, color) != 0) return 1;

    // Pontos 2, 3 e 4
    uint32_t interrupts = 0;                    // gere a quantidade de interrupções recebidas
    uint32_t draw_second_square = 30 * delay;   // desenha o segundo quadrado quando passarem delay segundos
    uint32_t stop = 30 * delay * 2;             // acaba o ciclo quando passarem 2 * delay segundos

    while (interrupts != stop) { // o ciclo termina quando passarem 2 * delay segundos

        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:                        
                    if (msg.m_notify.interrupts & LCV1_MASK) { // interrupção do dispositivo
                        interrupts++;
                        if (interrupts == draw_second_square) {
                            // quando passarem os primeiros delay segundos desenha o novo quadrado
                            if (pp_draw_square(y, x, len, color) != 0) return 1;
                        }          
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // Voltar ao modo texto, modo 0. Sem interrupções.
    if (pp_set_mode(0, 0) != 0) return 1;

    return 0;
}
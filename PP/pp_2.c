// Resolução PP2 2020 - The LPV-API
// Fábio Sá 2023

#include <stdlib.h>
#include <stdint.h>

// Macros úteis
#define BIT(N) (1 << (N))

// Variáveis globais
uint8_t *frame_buffer;
lpv_mode_info_t mode_info;
uint8_t bytes_per_pixel;

int draw_pixel(uint16_t x, uint16_t y, uint32_t color) {
    int offset = ((y * mode_info.x_res) + x) * bytes_per_pixel;
    if (memcpy(&frame_buffer[offset], &color, bytes_per_pixel) == NULL) return 1;
    return 0;
}

int normalize_color(uint8_t mode, uint32_t color, uint32_t *new_color) {

    switch(mode) {
        case 0: // Modo texto -> não faz nada
            *new_color = color;
            break;
        case 1: // Modo indexado -> não faz nada
            *new_color = color;
            break;
        case 2: // Modo ((8):8:8:8) -> os 8 bits mais significativos devem ficar a 0
            *new_color = color & (BIT(24) - 1);
            break;
        case 3: // Modo (5:6:5) -> é necessário fazer recortes
            uint32_t red =   (color & 0x001F0000) >> 5;
            uint32_t green = (color & 0x00003F00) >> 3;
            uint32_t blue =   color & 0x0000001F;
            *new_color = red | green | blue;
            break;
        case 4: // Modo (8:8:8) -> não faz nada
            *new_color = color;
            break;
        default:
            return 1;
    }
    return 0;
}

int pp_test_line(uint8_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay) {

    // Buscar informações do modo
    if (lpv_get_mode_info_t(mode, &mode_info) != 0) return 1;

    // Mapear a memória física e virtual
    bytes_per_pixel = (mode_info.bpp + 7) / 8;
    size_t size = mode_info.x_res * mode_info.y_res * bytes_per_pixel;
    frame_buffer = video_map_phys(mode_info.phys_addr, size);
    if (frame_buffer == NULL) return 1;

    // Mudar de modo texto para gráfico
    if (lpv_set_mode(mode) != 0) return 1;

    // Normalização da cor
    uint32_t new_color;
    if (normalize_color(mode, color, &new_color) != 0) return 1;

    // Ver qual é a direção da linha e desenhar com incremento
    switch (dir) {
        case lpv_hor:
            for (int i = 0 ; i < len ; i++) {
                if (draw_pixel(x + len, y, new_color) != 0) return 1;
            }
            break;
        case lpv_vert:
            for (int i = 0 ; i < len ; i++) {
                if (draw_pixel(x, y + len, new_color) != 0) return 1;
            }
            break;
    }

    // Espera delay segundos antes de mudar novamente o modo
    sleep(delay);

    // Muda de modo gráfico para texto
    if (lpv_set_mode(0) != 0) return 1;

    return 0;
}
#include "menu.h"
#include "controllers/video/graphics.h"
#include "controllers/mouse/mouse.h"

uint8_t *frame_buffer_1;
uint8_t *frame_buffer_2;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
MenuState menuState = START;

void draw_frame() {
    switch (menuState) {
        case START:
            draw_initial_menu();
            break;
        case GAME:
            draw_game_menu();
            break;
        case END:
            draw_finish_menu();
            break;
    }
    draw_mouse();
    swap_buffers();
}

void draw_initial_menu() {
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0xff0000, frame_buffer_1);
}

void draw_game_menu() {
    draw_rectangle(0, 0, mode_info.XResolution/2, mode_info.YResolution/2, 0xF01919, frame_buffer_1);
    draw_rectangle(mode_info.XResolution/2, 0, mode_info.XResolution/2, mode_info.YResolution/2, 0x19DBF0, frame_buffer_1);
    draw_rectangle(0, mode_info.YResolution/2, mode_info.XResolution/2, mode_info.YResolution/2, 0x2FF019, frame_buffer_1);
    draw_rectangle(mode_info.XResolution/2, mode_info.YResolution/2, mode_info.XResolution/2, mode_info.YResolution/2, 0xF5DA2A, frame_buffer_1);
}

void draw_finish_menu() {
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000ff, frame_buffer_1);
}

void draw_mouse() {
    draw_rectangle(mouse_info.x, mouse_info.y, 20, 20, 0, frame_buffer_1);
}

int set_frame_buffers(uint16_t mode) {
    if (set_frame_buffer(mode, &frame_buffer_1)) return 1;
    if (set_frame_buffer(mode, &frame_buffer_2)) return 1;
    return 0;
}

void swap_buffers() {
    uint8_t *temp = frame_buffer_1;
    frame_buffer_1 = frame_buffer_2;
    frame_buffer_2 = temp;
}

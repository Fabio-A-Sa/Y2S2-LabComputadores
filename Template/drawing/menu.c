#include "menu.h"
#include "controllers/video/graphics.h"

extern vbe_mode_info_t mode_info;
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
    swap_buffers();
}

void draw_initial_menu() {
    //printf("Drawing initial menu, with fb1 = %p and fb2 = %p\n", frame_buffer_1, frame_buffer_2);
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0xff0000, frame_buffer_1);
}

void draw_game_menu() {
    //printf("Drawing initial menu, with fb1 = %p and fb2 = %p\n", frame_buffer_1, frame_buffer_2);
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x00ff00, frame_buffer_1);
}

void draw_finish_menu() {
    //printf("Drawing initial menu, with fb1 = %p and fb2 = %p\n", frame_buffer_1, frame_buffer_2);
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000ff, frame_buffer_1);
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

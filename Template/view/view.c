#include "view.h"
#include "config.h"
#include "controller/video/graphics.h"
#include "controller/mouse/mouse.h"

uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *drawing_frame_buffer;
uint32_t frame_buffer_size;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
MenuState menuState = START;

int set_frame_buffers(uint16_t mode) {
    if (set_frame_buffer(mode, &main_frame_buffer)) return 1;
    frame_buffer_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel + 7) / 8);
    if (DOUBLE_BUFFER) {
        secondary_frame_buffer = (uint8_t *) malloc(frame_buffer_size);
        drawing_frame_buffer = secondary_frame_buffer;
    } else {
        drawing_frame_buffer = main_frame_buffer;
    }
    return 0;
}

void swap_buffers() {
    memcpy(main_frame_buffer, secondary_frame_buffer, frame_buffer_size);
}

void draw_new_frame() {
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
}

void draw_initial_menu() {
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0xff0000, drawing_frame_buffer);
}

void draw_game_menu() {
    draw_rectangle(0, 0, mode_info.XResolution/2, mode_info.YResolution/2, 0xF01919, drawing_frame_buffer);
    draw_rectangle(mode_info.XResolution/2, 0, mode_info.XResolution/2, mode_info.YResolution/2, 0x19DBF0, drawing_frame_buffer);
    draw_rectangle(0, mode_info.YResolution/2, mode_info.XResolution/2, mode_info.YResolution/2, 0x2FF019, drawing_frame_buffer);
    draw_rectangle(mode_info.XResolution/2, mode_info.YResolution/2, mode_info.XResolution/2, mode_info.YResolution/2, 0xF5DA2A, drawing_frame_buffer);
}

void draw_finish_menu() {
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x0000ff, drawing_frame_buffer);
}

void draw_mouse() {
    draw_rectangle(mouse_info.x, mouse_info.y, 20, 20, 0, drawing_frame_buffer);
}

#include "view.h"
#include "config.h"
#include "controller/video/graphics.h"
#include "controller/mouse/mouse.h"
#include "model/model.h"

uint8_t *main_frame_buffer;
uint8_t *secondary_frame_buffer;
uint8_t *drawing_frame_buffer;
uint32_t frame_buffer_size;
extern vbe_mode_info_t mode_info;
extern MouseInfo mouse_info;
MenuState menuState = START;

extern Sprite *mouse;
extern Sprite *hand;

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
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, RED, drawing_frame_buffer);
}

void draw_game_menu() {
    draw_rectangle(0, 0, mode_info.XResolution/2, mode_info.YResolution/2, ORANGE, drawing_frame_buffer);
    draw_rectangle(mode_info.XResolution/2, 0, mode_info.XResolution/2, mode_info.YResolution/2, BLUE, drawing_frame_buffer);
    draw_rectangle(0, mode_info.YResolution/2, mode_info.XResolution/2, mode_info.YResolution/2, GREEN, drawing_frame_buffer);
    draw_rectangle(mode_info.XResolution/2, mode_info.YResolution/2, mode_info.XResolution/2, mode_info.YResolution/2, YELLOW, drawing_frame_buffer);
}

void draw_finish_menu() {
    draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, DARKBLUE, drawing_frame_buffer);
}

void draw_mouse() {
    draw_sprite(mouse, mouse_info.x, mouse_info.y);
}

int draw_sprite(Sprite *sprite, int x, int y) { 
    uint16_t height = sprite->height;
    uint16_t width = sprite->width;
    uint32_t current_color;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        current_color = sprite->colors[w + h*width];
        if (current_color == 0xFFFFFE) continue;
        if (draw_pixel(x + w, y + h, current_color, drawing_frame_buffer) != 0) return 1;
      }
    }
    return 0; 
}

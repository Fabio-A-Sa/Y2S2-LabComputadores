#include "model.h"

extern uint8_t scancode;
extern uint8_t byte_index;
SystemState systemState = RUNNING;
extern MenuState menuState;
extern MouseInfo mouse_info;
extern vbe_mode_info_t mode_info;

Sprite *mouse;
Sprite *hand;
Sprite *button1;
Sprite *button2;
Sprite *button3;
Sprite *button4;

void setup_sprites() {
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    hand = create_sprite_xpm((xpm_map_t) hand_xpm);
    button1 = create_sprite_button(mode_info.XResolution/2, mode_info.YResolution/2, ORANGE);
    button2 = create_sprite_button(mode_info.XResolution/2, mode_info.YResolution/2, BLUE);
    button3 = create_sprite_button(mode_info.XResolution/2, mode_info.YResolution/2, GREEN);
    button4 = create_sprite_button(mode_info.XResolution/2, mode_info.YResolution/2, YELLOW);
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(hand);
    destroy_sprite(button1);
    destroy_sprite(button2);
    destroy_sprite(button3);
    destroy_sprite(button4);
}

void update_timer_state() {
    draw_new_frame();
    if (DOUBLE_BUFFER) swap_buffers();
}

void update_keyboard_state() {
    (kbc_ih)();
    switch (scancode) {
        case Q_KEY:
            systemState = EXIT;
            break;
        case S_KEY:
            menuState = START;
            break;
        case G_KEY:
            menuState = GAME;
            break;
        case E_KEY:
            menuState = END;
        default:
            break;
    }
}

void update_mouse_state() {
    (mouse_ih)();
    mouse_sync_bytes();
    if (byte_index == 3) {
        mouse_sync_info();
        update_buttons_state();
        byte_index = 0;
    }
}

void update_buttons_state() {

    if (mouse_info.left_click) {

        if (mouse_info.x < mode_info.XResolution/2 && mouse_info.y < mode_info.YResolution/2)
            button1->pressed = 1;

        if (mouse_info.x >= mode_info.XResolution/2 && mouse_info.y <= mode_info.YResolution/2)
            button2->pressed = 1;

        if (mouse_info.x < mode_info.XResolution/2 && mouse_info.y >= mode_info.YResolution/2)
            button3->pressed = 1;

        if (mouse_info.x >= mode_info.XResolution/2 && mouse_info.y > mode_info.YResolution/2)
            button4->pressed = 1;

    } else {
        button1->pressed = 0;
        button2->pressed = 0;
        button3->pressed = 0;
        button4->pressed = 0;
    }
}

#include "model.h"

extern uint8_t scancode;
extern uint8_t byte_index;
SystemState systemState = RUNNING;
extern MenuState menuState;

Sprite *mouse;
Sprite *hand;

void setup_sprites() {
    mouse = create_sprite((xpm_map_t) mouse_xpm);
    hand = create_sprite((xpm_map_t) hand_xpm);
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(hand);
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
        byte_index = 0;
    }
}

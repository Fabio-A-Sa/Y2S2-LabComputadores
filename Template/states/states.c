#include "states.h"
#include "controllers/keyboard/KBC.h"
#include "controllers/mouse/mouse.h"
#include "drawing/menu.h"

extern uint8_t scancode;
extern uint8_t byte_index;
SystemState systemState = RUNNING;
extern MenuState menuState;

void update_timer_state() {
    swap_buffers();
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
    draw_new_frame();
}

void update_mouse_state() {
    (mouse_ih)();
    mouse_sync_bytes();
    if (byte_index == 3) {
        mouse_sync_info();
        byte_index = 0;
        draw_new_frame();
    }
}

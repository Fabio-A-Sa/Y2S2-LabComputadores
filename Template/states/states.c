#include "states.h"
#include "controllers/keyboard/KBC.h"
#include "controllers/mouse/mouse.h"
#include "drawing/menu.h"

extern uint8_t scancode;
extern uint8_t byte_index;
SystemState systemState = RUNNING;
extern MenuState menuState;
uint8_t new_frame = 0;

void update_timer_state() {
    if (new_frame) {
        draw_frame();
        new_frame = 0;
    }
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
    new_frame = 1;
}

void update_mouse_state() {
    (mouse_ih)();
    mouse_sync_bytes();
    if (byte_index == 3) {
        mouse_sync_info();
        byte_index = 0;
        new_frame = 1;
    }
}

#include "states.h"
#include "controllers/keyboard/KBC.h"
#include "controllers/mouse/mouse.h"
#include "drawing/menu.h"

extern uint8_t scancode;
extern uint8_t byte_index;
extern struct packet mouse_packet;
SystemState systemState = RUNNING;
extern MenuState menuState;

void update_timer_state() {
    draw_frame();
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
        byte_index = 0;
        mouse_bytes_to_packet();
        printf("Mouse up with delta_x = %d\n", mouse_packet.delta_x);
    }
}

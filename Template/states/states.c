#include "states.h"
#include "controllers/keyboard/KBC.h"
#include "controllers/mouse/mouse.h"
#include "drawing/menu.h"

int timer_counter = 0;
extern uint8_t scancode;
extern uint8_t byte_index;
extern struct packet mouse_packet;
SystemState systemState = RUNNING;

void update_timer_state() {
    timer_counter++;
    if (timer_counter % 30 == 0) { //GAME_FPS
        draw_frame();
    }
}

void update_keyboard_state() {
    (kbc_ih)();
    if (scancode == BREAK_ESC) {
        systemState = EXIT;
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

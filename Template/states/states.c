#include "states.h"
#include "controllers/keyboard/KBC.h"
#include "controllers/mouse/mouse.h"

int timer_counter = 0;
extern uint8_t scancode;
extern uint8_t byte_index;
extern struct packet mouse_packet;
SystemState systemState = RUNNING;

void update_timer_state() {
    timer_counter++;
    if (timer_counter % 30 == 0) {
        printf("Timer up\n");
        systemState = EXIT;
    }
}

void update_keyboard_state() {
    (kbc_ih)();
    printf("Keyboard up with scancode = %d\n", scancode);
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

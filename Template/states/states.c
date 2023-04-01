#include "states.h"

int timer_counter = 0;
SystemState systemState = RUNNING;

void update_timer_state() {
    timer_counter++;
    if (timer_counter % 30 == 0) {
        printf("Timer up\n");
    }
}

void update_keyboard_state() {
    printf("Keyboard up\n");
}

void update_mouse_state() {
    printf("Mouse up\n");
}

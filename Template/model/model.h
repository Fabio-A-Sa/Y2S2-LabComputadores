#ifndef _LCOM_STATES_H_
#define _LCOM_STATES_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

typedef enum {
    RUNNING,
    EXIT,
} SystemState;

void update_timer_state();
void update_keyboard_state();
void update_mouse_state();

int setup_sprites();

#endif

#ifndef _LCOM_STATES_H_
#define _LCOM_STATES_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard/KBC.h"
#include "controller/mouse/mouse.h"
#include "xpm/hand.xpm"
#include "xpm/mouse.xpm"
#include "view/view.h"
#include "model/sprite.h"
#include "config.h"

typedef enum {
    RUNNING,
    EXIT,
} SystemState;

void update_timer_state();
void update_keyboard_state();
void update_mouse_state();
void setup_sprites();
void destroy_sprites();

#endif
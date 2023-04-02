#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

typedef enum {
    START,
    GAME,
    END
} MenuState;

void draw_frame();
void draw_initial_menu();
void draw_game_menu();
void draw_finish_menu();
void draw_mouse();

int set_frame_buffers(uint16_t mode);
void swap_buffers();

#endif
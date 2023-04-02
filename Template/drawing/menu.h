#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

typedef enum {
    START,
    GAME,
    END
} MenuState;

uint8_t *frame_buffer_1;
uint8_t *frame_buffer_2;

void draw_frame();
void draw_initial_menu();
void draw_game_menu();
void draw_finish_menu();

int set_frame_buffers(uint16_t mode);
void swap_buffers();

#endif

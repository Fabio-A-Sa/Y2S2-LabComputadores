#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include "model/sprite.h"
#include <lcom/lcf.h>

void draw_new_frame();
void draw_initial_menu();
void draw_game_menu();
void draw_finish_menu();
void draw_mouse();
void swap_buffers();
int draw_sprite_xpm(Sprite *sprite, int x, int y);
int draw_sprite_button(Sprite *sprite, int x, int y);
int set_frame_buffers(uint16_t mode);

#endif

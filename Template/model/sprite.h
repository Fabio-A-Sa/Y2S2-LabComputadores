#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include "xpm/hand.xpm"
#include "xpm/mouse.xpm"

typedef struct {
    uint16_t height
    uint16_t width;
    uint32_t *colors;
} Sprite; 

Sprite *mouse;
Sprite *hand;

Sprite *createSprite(xpm_map_t sprite);
int drawSprite(Sprite *sp, int x, int y);
void destroy_sprites();
void loadSprites();

#endif

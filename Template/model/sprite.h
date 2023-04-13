#ifndef _LCOM_SPRITE_H_
#define _LCOM_SPRITE_H_

#include "controller/video/graphics.h"

typedef struct {
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
} Sprite; 

Sprite *create_sprite(xpm_map_t sprite);
void destroy_sprite(Sprite *sprite);

#endif

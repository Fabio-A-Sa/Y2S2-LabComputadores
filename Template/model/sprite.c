#include "sprite.h"

Sprite *create_sprite(xpm_map_t sprite){

  Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
  if( sp == NULL ) return NULL;

  xpm_image_t img;
  sp->colors = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);
  sp->height = img.height;
  sp->width = img.width;

  if( sp->colors == NULL ) {
    free(sp);
    return NULL;
  }

  return sp;
}

void destroy_sprite(Sprite *sprite) {
    if( sprite == NULL ) {
      return;
    }
    else if( sprite->colors){
        free(sprite->colors);
    }
    
    free(sprite);
    sprite = NULL;
}

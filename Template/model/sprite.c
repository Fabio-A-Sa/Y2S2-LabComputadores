#include "sprite.h"

// Construção de um Sprite com um XPM com cores em modo direto
// O Sprite terá de conter um array de cores
Sprite *create_sprite_xpm(xpm_map_t sprite){

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

// Construção de um Sprite retangular com cor constante
// O Sprite não tem um array de cores, seria gastar memória desnecessariamente
Sprite *create_sprite_button(uint16_t width, uint16_t height, uint32_t color){
  Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
  sp->height = height;
  sp->width = width;
  sp->colors = NULL;
  sp->color = color;
  sp->pressed = 0;
  return sp;
}

// É boa prática antes de acabar o programa libertar a memória alocada
void destroy_sprite(Sprite *sprite) {
    if (sprite == NULL) return;
    if (sprite->colors) free(sprite->colors);
    free(sprite);
    sprite = NULL;
}

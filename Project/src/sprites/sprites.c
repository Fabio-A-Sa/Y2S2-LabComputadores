#include "sprites.h"

/*
Sprite *gameTitle;
Sprite *mouseCursor;
Sprite *brushCursor;
Sprite *handCursor;
Sprite *startButton;
Sprite *quitButton;
Sprite *instButton;
*/

//game condition
extern int gameMode;
//mouse externs
extern int x;
extern int y;
//display externs
extern uint8_t *imgBuffer;
extern uint16_t xRes, yRes;
extern unsigned bytesPerPixel;
//extern reader
extern char word[24];
//extern Menu
extern bool startDrawPressed;
extern bool startGuessPressed;
extern bool quitPressed;
extern bool instrPressed;

//tracking color place
int xColorSelector = 13;

Sprite *createSprite(xpm_map_t sprite){

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

int drawSprite(Sprite *sprite, int x, int y){ 
    uint16_t height = sprite->height, width = sprite->width;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (vg_draw_pixel(x + w, y + h, sprite->colors[w + h*width])) return 1;
      }
    }
    return 0; 
}

void loadSprites(){
  gameTitle = createSprite((xpm_map_t) title_xpm);
  mouseCursor = createSprite((xpm_map_t) mouseCursor_xpm);
  handCursor = createSprite((xpm_map_t) handCursor_xpm);
  brushCursor = createSprite((xpm_map_t) brushCursor_xpm);
  drawButton = createSprite((xpm_map_t) drawButton_xpm);
  guessButton = createSprite((xpm_map_t) guessButton_xpm);
  quitButton = createSprite((xpm_map_t) quitButton_xpm);
  instButton = createSprite((xpm_map_t) instButton_xpm);
}

void unloadSprites(){
  destroy_sprite(gameTitle);
  destroy_sprite(mouseCursor);
  destroy_sprite(brushCursor);
  destroy_sprite(handCursor);
  destroy_sprite(drawButton);
  destroy_sprite(guessButton);
  destroy_sprite(quitButton);
  destroy_sprite(instButton);
}

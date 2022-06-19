/**
 * @file sprites
 * @brief This file contains the functions necessary to create, destroy, load, unload and print a sprite to the display.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "../controllers/video/graphics.h"
#include "../imgs/title.xpm"
#include "../imgs/mice/mouseCursor.xpm"
#include "../imgs/mice/brushCursor.xpm"
#include "../imgs/mice/handCursor.xpm"
#include "../imgs/buttons/drawButton.xpm"
#include "../imgs/buttons/guessButton.xpm"
#include "../imgs/buttons/quitButton.xpm"
#include "../imgs/buttons/instButton.xpm"
#include "../imgs/buttons/drawButtonPressed.xpm"
#include "../imgs/buttons/guessButtonPressed.xpm"
#include "../imgs/buttons/quitButtonPressed.xpm"
#include "../imgs/buttons/instButtonPressed.xpm"
#include "../imgs/instPage.xpm"
#include "../imgs/brushPlus.xpm"
#include "../imgs/brushMinus.xpm"

#include "../imgs/numbers/zero.xpm"
#include "../imgs/numbers/one.xpm"
#include "../imgs/numbers/two.xpm"
#include "../imgs/numbers/three.xpm"
#include "../imgs/numbers/four.xpm"
#include "../imgs/numbers/five.xpm"
#include "../imgs/numbers/six.xpm"
#include "../imgs/numbers/seven.xpm"
#include "../imgs/numbers/eight.xpm"
#include "../imgs/numbers/nine.xpm"
#include "../imgs/letters/a.xpm"
#include "../imgs/letters/b.xpm"
#include "../imgs/letters/c.xpm"
#include "../imgs/letters/d.xpm"
#include "../imgs/letters/e.xpm"
#include "../imgs/letters/f.xpm"
#include "../imgs/letters/g.xpm"
#include "../imgs/letters/h.xpm"
#include "../imgs/letters/i.xpm"
#include "../imgs/letters/j.xpm"
#include "../imgs/letters/k.xpm"
#include "../imgs/letters/l.xpm"
#include "../imgs/letters/m.xpm"
#include "../imgs/letters/n.xpm"
#include "../imgs/letters/o.xpm"
#include "../imgs/letters/p.xpm"
#include "../imgs/letters/q.xpm"
#include "../imgs/letters/r.xpm"
#include "../imgs/letters/s.xpm"
#include "../imgs/letters/t.xpm"
#include "../imgs/letters/u.xpm"
#include "../imgs/letters/v.xpm"
#include "../imgs/letters/w.xpm"
#include "../imgs/letters/x.xpm"
#include "../imgs/letters/y.xpm"
#include "../imgs/letters/z.xpm"

#include "../imgs/symbols/plus.xpm"
#include "../imgs/symbols/minus.xpm"

#include "../controllers/video/COLORS.h"
#include "../game/UTILS.h"

#include "../game/reader.h"

/**
 * @brief Sprite stores information about an image that is allocate in memomry
 * @param height The height on the image in pixels
 * @param width The width of the image in pixels
 * @param colors The array that stores the colors of each pixel in the image 
 * 
 */
typedef struct Sprite{
    uint16_t height, width;
    uint32_t *colors;
} Sprite; 

Sprite *gameTitle;
Sprite *mouseCursor;
Sprite *brushCursor;
Sprite *handCursor;
Sprite *drawButton;
Sprite *guessButton;
Sprite *quitButton;
Sprite *instButton;

/**
 * @brief Create a Sprite object from a xpm
 * 
 * @param sprite The xmp to be loaded as an image
 * @return An object of type Sprite representing the image 
 */
Sprite *createSprite(xpm_map_t sprite);

/**
 * @brief Draws a loaded SPrite onto the screen, The x and y represents the top left corner of the Sprite
 * 
 * @param sp The Sprite to be drawn of the sreen
 * @param x The first x position to put the Sprite (left to right)
 * @param y The first y position to put the Sprite (top to bottom)
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int drawSprite(Sprite *sp, int x, int y);

/**
 * @brief A function that unloaeds and frees the memory the Sprite was using
 * 
 * @param sp Sprite to be unloaded
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief A function that loads all the needed Sprites when te program starts running.
 * allocates memory as needed
 */
void loadSprites();

/**
 * @brief A function that loads all the needed Sprites when te program starts running,
 * it also frees the memory they were using
 */
void unloadSprites();

#endif

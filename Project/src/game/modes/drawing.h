/**
 * @file drawing
 * @brief This file contains the functions necessary to process the events that occurs in drawing mode.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "../../controllers/keyboardMouse/i8042.h"
#include "../../controllers/video/COLORS.h"
#include "../../controllers/video/graphics.h"
#include "../reader.h"
#include "../../sprites/sprites.h"

/**
 * @brief This function process left clicks on the draw page, it verifeis where the player clicked and realizes the action accordingly.
 * 
 * @param x x position of the mouse during the click
 * @param y y position of the mouse during the click
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int processGamingLeftClick(int x, int y);

/**
 * @brief This function process right clicks on the draw page, it verifeis where the player clicked and realizes the action accordingly.
 * 
 * @param x x position of the mouse during the click
 * @param y y position of the mouse during the click
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int processGamingRightClick(int x, int y);

/**
 * @brief This funtion process the word selected for the gameplay and displays it to the screen when called.
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int processWord();

/**
 * @brief This function process the pressing of jeys by the player and realizes the function correct action for the key pressed.
 * @param scancode Code representing the key pressed by the player
 */
void KB_DrawingControls(uint8_t scancode);

/**
 * @brief This function loads the differents components of a frame into the secondary buffer
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int drawDrawingGame();

#endif /* _DRAWING_H_ */

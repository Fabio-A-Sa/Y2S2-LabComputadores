/**
 * @file menu
 * @brief This file contains the functions necessary to process the events that occurs when in the menu of the game.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef __MENU_H
#define __MENU_H

#include "../../controllers/video/COLORS.h"
#include "../../controllers/video/graphics.h"
#include "drawing.h"
#include "../../controllers/keyboardMouse/mouse.h"

/**
 * @brief This function process the pressing of jeys by the player and realizes the function correct action for the key pressed.
 * @param scancode Code representing the key pressed by the player
 */
void KB_MenuControls(uint8_t scancode);

/**
 * @brief This function detects it the player is pressing the draw button and if the press if valid. If it is, it starts draw mode. 
 * 
 * @param x x position of the mouse during the click
 * @param y y position of the mouse during the click
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int processStartDrawButton(uint16_t x, uint16_t y);

/**
 * @brief This function detects it the player is pressing the guess button and if the press if valid. If it is, it starts guess mode. 
 * 
 * @param x x position of the mouse during the click
 * @param y y position of the mouse during the click
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int processStartGuessButton(uint16_t x, uint16_t y);

/**
 * @brief This function detects it the player is pressing the quit button and if the press if valid. If it is, it exits the game. 
 * 
 * @param x x position of the mouse during the click
 * @param y y position of the mouse during the click
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int processQuitButton(uint16_t x, uint16_t y);

/**
 * @brief This function detects it the player is pressing the instruction button and if the press if valid. If it is, it loads the instruction page. 
 * 
 * @param x x position of the mouse during the click
 * @param y y position of the mouse during the click
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int processInstrButton(uint16_t x, uint16_t y);

/**
 * @brief This function loads the differents components of a frame into the secondary buffer
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int drawMenu();
    
#endif

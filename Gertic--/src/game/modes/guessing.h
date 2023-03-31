/**
 * @file guessing
 * @brief This file contains the functions necessary to process the events that occurs in guessing mode.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _GUESSING_H_
#define _GUESSING_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../../controllers/keyboardMouse/i8042.h"
#include "../../controllers/video/COLORS.h"
#include "../../controllers/video/graphics.h"
#include "../../sprites/sprites.h"


/**
 * @brief This function process the pressing of jeys by the player and realizes the function correct action for the key pressed.
 * @param scancode Code representing the key pressed by the player
 */
void KB_GuessingControls(uint8_t scancode);

/**
 * @brief This function is called to compare the input of the player and the word given. It checks id the guess is correct or not.
 * @return int 0 if the guess is correct, 0 if otherwise
 */
int compareGuess();

/**
 * @brief This function loads the differents components of a frame into the secondary buffer
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int drawGuessingGame();

#endif /* _GUESSING_H_ */

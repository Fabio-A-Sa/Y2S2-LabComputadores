/**
 * @file inst
 * @brief This file contains the functions necessary to process the instruction page of the game.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _INST_H_
#define _INST_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "../../controllers/keyboardMouse/i8042.h"
#include "../../controllers/video/COLORS.h"
#include "../../controllers/video/graphics.h"
#include "../../sprites/sprites.h"

/**
 * @brief This function detects if the player pressed esc and exits the instruction page
 * @param scancode Scan code representing the key the player pressed
 */
void KB_InstControls(uint8_t scancode);

/**
 * @brief This funtions load the instruciton page (a xpm) to the tertiaty buffer (or "static elements" buffer).
 * @return Return 0 if there is no error, 1 if otherwise 
 */
int drawInstPage();

#endif /* _INST_H_ */

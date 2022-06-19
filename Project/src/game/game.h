/**
 * @file game
 * @brief This file contains functions that allows drawing the mouse independently of the mode the player is,
 * the intepretation of the words (given word to player drawing or input from player guessing), and the counter of the game.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _GAME_H_
#define _GAME_H_

#include <stdint.h>

/**
 * @brief This function prints the xpm of the mouse on the screen, it checks to see with mouse xpm it should use depending on its place on the screen
 */
void (printMouse)();

/**
 * @brief This function reads the array of the word given to the draw user and prints the xpm of the letter at the appropiate place on the screen
 * @param word Text to bet written (printed) on the screen
 * @param yLevel The heigh at the screen where the text will be written
 * @return Return 0 if there is no error, 1 if otherwise
 */
int wordInterpreter(char *word, int yLevel);

/**
* @brief This funtion draws the in-game counter on the screen
 * @return Return 0 if there is no error, 1 if otherwise
*/
int gamePlayCounterDraw();

#endif /* _GAME_H_ */

/**
 * @file reader
 * @brief This file contains the function that reads from the document that contains a list of words to give to player in draw mode.
 * It selectes a random word from the file.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _READER_H_
#define _READER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <lcom/lcf.h>

#define WORDS "wordList.txt"

/**
* @brief This function reads a random word from the file containing the list of words that a player has to draw (or guess)
* @return Return 0 if there is no error, 1 if otherwise
*/
int readWord();

#endif /* _READER_H_ */

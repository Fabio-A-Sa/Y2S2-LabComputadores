/**
 * @file serialPort
 * @brief This file contains the functions that allows for the configuration and interruptions of the serial Port.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 06/06/2022
 */

#ifndef _SERIAL_PORT_
#define _SERIAL_PORT_

#include <lcom/lcf.h>
#include "queue.h"
#include "SP.h"

#define SP_ATTEMPS 5

/**
* @brief This function subscribes serial port interrupts with policies REENABLE and EXCLUSIVE
* @param [out] bit_no All zeros except for the bit position of the serial port's hook id
* @return Return 0 if case of success, 1 if otherwise
*/
int sp_subscribe_int(uint8_t* bitno);

/**
* @brief This function unsubscribes serial port interrupts
* @return Return 0 if case of success, 1 if otherwise
*/
int sp_unsubscribe_int();

/**
* @brief This function reads the status of the serial port
* @param [out] status The current status of the serial port
* @return Return 0 if case of success, 1 if otherwise
*/
int sp_get_status(uint8_t* status);

/**
* @brief This function initializes the serial port, enabling interrupts and creating the received data queue
*/
void sp_init();

/**
* @brief This function exits the serial port by clearing the received data queue
*/
void sp_exit();

/**
* @brief Serial ports's interrupt handler
*/
void sp_ih();


/**
* @brief This function sends a byte through the serial port
* @param [out] byte The byte to send
* @return Return 0 if case of success, 1 if otherwise
*/
int send_byte(uint8_t byte);

/**
* @brief This function reads a byte from the serial port and pushes it to the received data queue
* @return Return 0 if case of success, 1 if otherwise
*/
int read_byte();

/**
* @brief This function is used to get the received data queue
* @return Pointer to the received data queue
*/
Queue* get_inQueue();

/**
 * @brief The function prepares the queue and FCR for the next interruptiom, it empties the queue to prevent errors in the next use
 * @return Return 0 if case of success, 1 if otherwise
 */
int SP_clearInterrupts();

/**
* @brief This function determines what a given byte received from the serial port should be used for while waiting for the game to start in drawing mode.
* @param [in] val The received byte
* @return Return 0 if case of success, 1 if otherwise
*/
int processWaitingDrawSP(uint8_t val);

/**
* @brief This function determines what a given byte received from the serial port should be used for while waiting for the game to start in guess mode.
* @param [in] val The received byte
* @return Return 0 if case of success, 1 if otherwise
*/
int processWaitingGuessSP(uint8_t val);

/**
* @brief This function determines what a given byte received from the serial port should be used for while loading in guess mode.
* @param [in] val The received byte
* @return Return 0 if case of success, 1 if otherwise
*/
int processLoadingGuessSP(uint8_t val);

/**
* @brief This function determines what a given byte received from the serial port should be used for while playing in draw mode.
* @param [in] val The received byte
* @return Return 0 if case of success, 1 if otherwise
*/
int processPlayingDrawSP(uint8_t val);

/**
* @brief This function determines what a given byte received from the serial port should be used for while playing in guess mode.
* @param [in] val The received byte
* @return Return 0 if case of success, 1 if otherwise
*/
int processPlayingGuessSP(uint8_t val);

#endif // _SERIAL_PORT_

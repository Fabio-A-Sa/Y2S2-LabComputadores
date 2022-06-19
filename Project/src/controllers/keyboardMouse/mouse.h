/**
 * @file mouse
 * @brief This file contains the functions that allows configuring the mouse, invoke and process its interruptions, 
 * and synchronize information regarding the atualization of its position.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _MOUSE_
#define _MOUSE_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

/**
* @brief this function configures the mouse, verify if there is no error with the read of the port
* @param [in] controlWord Word to be sent to the port
* @return Return 0 in case of success, 1 otherwise
*/
int (mouse_config)(uint8_t controlWord);

/**
* @brief this function writes a specified controlWord to port defined by the user
* @param port Port where the information is goint to be written to
* @param controlWord Word (uint8_t) to be sent to the port
* @return Return 0 if case of success, 1 if otherwise
*/
int (KBCWrite)(uint8_t port, uint8_t controlWord);

/**
* @brief this function reads from a port and put the word read to output
* @param port Port where the information is goint to be read from
* @param [out] output variable where the information read is going to be stored
* @return Return 0 if case of success, 1 if otherwise
*/
int (KBCRead)(uint8_t port, uint32_t *output);

/**
 * @brief This function uses irq_setpolicy to configure mouse interrupts
 * @param [out] bit_no All zeros except for the bit position of the timer's hook id
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mouse_subscribe)(uint8_t *bit_no);

/**
 * @brief Mouse's input handler
 *
 */
void (mouse_ih)();

/**
 * @brief This function unsubscribes mouse interrupts
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mouse_unsubscribe)();

/**
 * @brief This function verifies the mouse interrupt and appropiately stores it in an array for mouse
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mouseSync)();

/**
 * @brief When mouse packet array has stored all the necessary bytes this functions interprets it and updates mouse information
 * @return Return 0 if case of success, 1 if otherwise
 */
int (mousePacket)();

/**
 * @brief This funtionc uses the information processed by mousePacket to update mouse location thaking into account the size and boundaries of the screen
 */
void updateMouseLocation();

#endif /* _MOUSE_ */

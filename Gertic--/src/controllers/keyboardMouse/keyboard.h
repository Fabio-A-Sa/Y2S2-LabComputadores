/**
 * @file keyboard
 * @brief This file contains the functions that allows configuring the keyboard, as well, as invoke and process its interruptions.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

/**
* @brief This function subscribes keyboard interrupts with policies REENABLE and EXCLUSIVE
* @param [out] bit_no All zeros except for the bit position of the keyboard's hook id
* @return Return 0 if case of success, 1 if otherwise
*/
int (subscribe_KBC_interrupts)(uint8_t *bit_no);

/**
* @brief This function unsubscribes keyboard interrupts
* @return Return 0 if case of success, 1 if otherwise
*/
int (unsubscribe_KBC_interrupts)();

/**
* @brief This function reads the status of the KBC
* @param [out] status The current status of the KBC
* @return Return 0 if case of success, 1 if otherwise
*/
int (read_KBC_status)(uint8_t* status);

/**
* @brief This function reads the content in the specified port
* @param [in] port Port to read from
* @param [out] output The data present in the given port
* @return Return 0 if case of success, 1 if otherwise
*/
int (read_KBC_output)(uint8_t port, uint8_t *output);

/**
* @brief This function writes a command to the specified port
* @param [in] port Port to write to
* @param [in] commandByte The command byte to write
* @return Return 0 if case of success, 1 if otherwise
*/
int (write_KBC_command)(uint8_t port, uint8_t commandByte);

/**
* @brief Keyboard's interrupt handler
*/
void (kbc_ih)();

/**
* @brief Restore the KBC configuration to MINIX's default
* @return Return 0 if case of success, 1 if otherwise
*/
int (kbc_restore)();

#endif /* _LCOM_KBC_H_ */

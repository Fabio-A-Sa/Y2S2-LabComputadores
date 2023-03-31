/**
 * @file timer
 * @brief This file contains the functions that allows for the configuration of the timer. Interruptions
 * and synchronization of information as requested.
 *
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
* @brief This function uses irq_setpolicy to configure timer interrupts
* @param [out] bit_no
* @return Return 0 in case of success, 1 if otherwise
*/
int (timer_subscribe_int)(uint8_t *bit_no);

/**
* @brief This function unsubscribes timer interrupts
* @return Return 0 in case of success, 1 if otherwise
*/
int (timer_unsubscribe_int)();

/**
* @brief Timer's interrupt handler, increments the counter
*/
void (timer_int_handler)();

/**
* @brief this function reads from the timer given by input its status
* @param [in] timer specifies the timer used
* @param [out] st variable where the information read will be stored 
* @return Return 0 in case of sucess, 1 otherwise
*/
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
* @brief 
* @param [in] timer
* @param [in] st
* @param [out] field
* @return Return 0 in case of success, 1 otherwise
*/
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

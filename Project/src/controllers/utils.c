/**
 * @file utils
 * @brief This file contains the functions that automates some procedures when reading or writing to mapped memory of the drivers.
 * 
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar , Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 31/05/2022
 */

#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief This function sends the least significant bytes of a 16 bit number;
 * @param val the number we want to send (not only the LSB)
 * @param lsb the port where to send it
 * @return Return 0 in case of success, 1 if otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  if (lsb == NULL) return 1;

  *lsb = 0xFF & val;
  return 0;
}

/**
 * @brief This function sends the most significant bytes of a 16 bit number;
 * @param val the number we want to send (not only the MSB)
 * @param lsb the port where to send it
 * @return Return 0 in case of success, 1 if otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  if (msb == NULL) return 1;

  *msb = (val >> 8) & 0xFF;
  return 0;
}

/**
 * @brief This function reads from the specified port and transforms the recevied value into an 8 bit number;
 * @param port the port where to read from
 * @param value the variable where to store the read value
 * @return Return 0 in case of success, 1 if otherwise
 */
int (util_sys_inb)(int port, uint8_t *value) {

  if (value == NULL) return 1;

  uint32_t val;
  int ret = sys_inb(port, &val);
  *value = 0xFF & val;
  return ret;
}

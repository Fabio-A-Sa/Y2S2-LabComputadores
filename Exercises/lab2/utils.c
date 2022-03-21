#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  *lsb = 0xFF & val;                    // Coloca em LSB os 8 bits menos significativos
  return 0;                             // Retorna se houve algum erro (0 -> false)
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  *msb = (val >> 8) & 0xFF;             // Coloca em MSB os 8 bits mais significativos
  return 0;                             // Retorna se houve algum erro (0 -> false)
}

int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t val;                         // instanciar uma variável: unsigned int de 32 bits
  int ret = sys_inb(port, &val);        // System call que irá colocar o conteúdo de port na zona de memória de val
  *value = 0xFF & val;                  // value ficará com os 8 bits menos significativos, ou seja, a control word do timer
  return ret;                           // Retorna se houve algum erro (0 -> false, 1 -> true)
}

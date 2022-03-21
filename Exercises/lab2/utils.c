#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  *lsb = 0xFF & val;
  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  *msb = (val >> 8) & 0xFF;
  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  int ret = sys_inb(port, &val);

  *value = 0xFF & val;

  return ret;
}

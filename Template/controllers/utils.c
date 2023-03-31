#include <lcom/lcf.h>
#include <stdint.h>

// LSB -> Less Significant Bits
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; // o apontador deve ser válido
  *lsb = 0xFF & val;         // preserva os 8 bits menos significativos
  return 0;
}

// MSB -> Most Significant Bits
int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
}

// Transform 32 bit output in 8 bit output. Error prevention.
int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) return 1;    // o apontador deve ser válido
  uint32_t val;                   // variável auxiliar de 32 bits
  int ret = sys_inb(port, &val);  // val ficará com o valor lido de port
  *value = 0xFF & val;            // value ficará apenas com os primeiros 8 bits do resultado
  return ret;
}

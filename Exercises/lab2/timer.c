#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  return 1;
}

int (timer_unsubscribe_int)() {
  return 1;
}

void (timer_int_handler)() {

}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  uint8_t controlWord = 0xC0;                               // 11000000 inicialmente {read-back command (11), ler a contagem atual (0) e ler o status (0)}
  if (timer >= 0 && timer < 3) {                            // se for um timer válido, {0..2}
  
      controlWord |= BIT(timer+1);                          // ativa o bit correspondente ao timer a buscar
      int ret = sys_outb(TIMER_CTRL, controlWord);          // system call para injetar a control word

      if (ret != 0) return ret                              // se ocorreu um erro na leitura, aborta a missão logo
      
      switch (timer) {
        case 0: ret = util_sys_inb(TIMER_0, st); break;    // 'st' fica com a configuração de timer 0
        case 1: ret = util_sys_inb(TIMER_1, st); break;    // 'st' fica com a configuração de timer 1
        case 2: ret = util_sys_inb(TIMER_2, st); break;    // 'st' fica com a configuração de timer 2
      }
      
      return ret;                                           // retorna a existência de um erro na leitura ou não

  } else return 1;                                          // número de timer inválido, aborta a missão logo
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val data;                        // variável que vai ficar com o valor pedido em 'field'

  switch (field) {

    case tsf_all: data.byte = st; break;                    // configuração toda
    case tsf_initial:                                       // modo de inicialização {none, LSB, MSB, MSB & LSB}

      if ((st & TIMER_LSB) == 0 && (st & TIMER_MSB) == 0): data.in_mode = INVAL_val;          // none
      if ((st & TIMER_LSB) != 0 && (st & TIMER_MSB) != 0): data.in_mode = MSB_after_LSB;      // MSB & LSB
      if ((st & TIMER_LSB) != 0 && (st & TIMER_MSB) == 0): data.in_mode = LSB_only;           // LSB
      if ((st & TIMER_LSB) == 0 && (st & TIMER_MSB) != 0): data.in_mode = MSB_only;           // MSB

      break;

    case tsf_mode:                                          // modo de contagem

      uint8_t mode = ((st >> 1) & 0x7);                     // ficar com os 3 bits que indicam o modo da contagem
                                                            // (st >> 1) & 00000111
      switch (mode) {
        case 0: data.count_mode = 0; break;                 // 000
        case 1: data.count_mode = 1; break;                 // 001
        case 2: case 6: data.count_mode = 2; break;         // X10
        case 3: case 7: data.count_mode = 3; break;         // X11
        case 4: data.count_mode = 4; break;                 // 100
        case 5: data.count_mode = 5; break;                 // 101
        default: return 1;                                  // valor inválido, aborta logo a missão
      }

      break;
    
    case tsf_base: data.bcd = st & TIMER_BCD; break;        // modo de escrita

    default: return 1;                                      // field inválido, aborta logo a missão
  }

  return timer_print_config(timer, st, data);               // printa a informação usando a função dos professores.
                                                            // retorna a existência de um erro ou não
}
#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
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


  return 1;
}

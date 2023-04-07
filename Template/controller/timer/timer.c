#include <lcom/lcf.h>
#include <stdint.h>
#include "timer.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  // Verificação de input
  if (freq > TIMER_FREQ || freq < 19) return 1;

  // Consultamos a configuração atual do @timer
  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord) != 0) return 1;

  // Novo comando de configuração, ativamos os bits da zona 'LSB followed by MSB' e mantemos os restantes
  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB; 

  // Cálculo do valor inicial do contador e partes mais e menos significativas
  uint32_t initialValue = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  util_get_MSB(initialValue, &MSB);
  util_get_LSB(initialValue, &LSB);

  // Atualização da controlWord de acordo com o timer escolhido
  // @selectedTimer ficará com a porta do timer escolhido (0x40, 0x41 ou 0x42)
  uint8_t selectedTimer;      
  switch (timer) {  
    case 0: 
      controlWord |= TIMER_SEL0;
      selectedTimer = TIMER_0;
      break;
    case 1:
      controlWord |= TIMER_SEL1;
      selectedTimer = TIMER_1;
      break;
    case 2:
      controlWord |= TIMER_SEL2;
      selectedTimer = TIMER_2;
      break;
    default:
      return 1;
  }

  // Avisamos o i8254 que vamos configurar o timer
  if (sys_outb(TIMER_CTRL, controlWord) != 0) return 1;

  // Injetamos o valor inicial do contador (lsb seguido de msb) diretamente no registo correspondente
  if (sys_outb(selectedTimer, LSB) != 0) return 1;
  if (sys_outb(selectedTimer, MSB) != 0) return 1;
  return 0;
}

int (timer_subscribe_interrupts)() {
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_interrupts)() {
  if (sys_irqrmpolicy(&hook_id) != 0) return 1; // desligar as interrupções
  return 0;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2 || timer < 0) return 1; // validação de input
  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer)); // construção do readback command
  if (sys_outb(TIMER_CTRL, RBC) != 0) return 1;       // avisar o i8254 que vamos ler a configuração
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;    // lemos a configuração diretamente do registo associado ao timer
  return 0;
}


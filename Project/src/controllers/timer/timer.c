#include "timer.h"

int hook_id = 0;
int counter_timer = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if (freq < 19 || freq > TIMER_FREQ) return 1;

  uint8_t controlWord;

  if (timer_get_conf(timer, &controlWord) != 0)
    return 1;

  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB; 

  uint32_t initialValue = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  util_get_MSB(initialValue, &MSB);
  util_get_LSB(initialValue, &LSB);

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
  }

  return sys_outb(TIMER_CTRL, controlWord) | sys_outb(selectedTimer, LSB) | sys_outb(selectedTimer, MSB);
}


int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL)
    return 1;
  
  *bit_no = BIT(hook_id);
  
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  counter_timer++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if (st == NULL || timer > 2 || timer < 0) return 1;

  uint8_t RBC = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if(sys_outb(TIMER_CTRL, RBC))
    return 1; 
  return util_sys_inb(TIMER_0 + timer, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val data;

  switch (field) {

    case tsf_all: data.byte = st; break;
    case tsf_initial:                                       
      st = (st >> 4) & 0x03;
 
      if (st == 1) data.in_mode = LSB_only;
      else if (st == 2) data.in_mode = MSB_only;
      else if (st == 3) data.in_mode = MSB_after_LSB;
      else data.in_mode = INVAL_val;
      
      break;

    case tsf_mode:
      st = (st >> 1) & 0x07;

      if(st == 6) data.count_mode = 2;
      else if(st == 7) data.count_mode = 3;
      else data.count_mode = st;

      break;
    
    case tsf_base:
      data.bcd = st & TIMER_BCD;
      break;        

    default:
      return 1;
  }

  return timer_print_config(timer, field, data);
}

#include "mouse.h"

int hook_id_mouse = 4;

int (mouse_subscribe)(uint8_t *bit_no){
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id_mouse);
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int (mouse_unsubscribe)(){
  return sys_irqrmpolicy(&hook_id_mouse);
}

void (mouse_ih)(){

}


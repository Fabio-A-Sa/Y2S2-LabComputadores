#include "port.h"

int hook_id_serial = 4;

int serial_subscribe_interrupts(){
  return sys_irqsetpolicy(IRQ_SERIAL, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_serial);
}

// Desativação das interrupções
int serial_unsubscribe_interrupts(){
  return sys_irqrmpolicy(&hook_id_serial);
}

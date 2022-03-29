#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8042.h"
#include "KBC.h"

#define hook_id = 1;

void sleep() {
    tickdelay(micros_to_ticks(WAIT_KBC));
}

int subscribe_KBC_interrupts() {
    return sys_irqsetpolicy(IRQ_KBC, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int unsubscribe_KBC_interrupts() {
    return sys_irqrmpolicy(&hook_id);
}

int readFromKBC() {

    return 1;
}

int writeToKBC() {

    return 1;
}
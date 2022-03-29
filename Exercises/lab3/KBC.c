#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"
#include "KBC.h"
#include "utils.c"

uint8_t scancode = 0x00;
uint8_t hook_id = 1;

void sleep() {
    tickdelay(micros_to_ticks(WAIT_KBC));
}

int subscribe_KBC_interrupts(uint8_t *bit_no) {

    *bit_no = BIT(hook_id);
    return sys_irqsetpolicy(IRQ_KBC, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int unsubscribe_KBC_interrupts() {
    return sys_irqrmpolicy(&hook_id);
}

int readFromKBC() {
    return util_sys_inb(KBC_OUT_BUF, &scancode);
}

int writeToKBC() {

    return 1;
}

void kbc_ih() {

    uint8_t status;
    if (util_sys_inb(KBC_ST_REG, &status) != 0) return 1;

    if (status & BIT(0)) { // o buffer de output está cheio

        // verifica a paridade correcta ou se houve timeout
        uint8_t someError = status & BIT(7) | status & BIT(6);

        if (readFromKBC() | someError) printf("Error in kbc_ih()\n");
    }
}
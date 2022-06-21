#include "KBC.h"
#include "i8042.h"

uint8_t scancode = 0;
int hook_id_KBC = 3;
uint8_t counter_KBC = 0;

int (subscribe_KBC_interrupts)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(hook_id_KBC);
    return sys_irqsetpolicy(IRQ_KBC, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_KBC);
}

int (unsubscribe_KBC_interrupts)() {
    return sys_irqrmpolicy(&hook_id_KBC);
}

int (read_KBC_status)(uint8_t* status) {
    return util_sys_inb(KBC_STATUS_REG, status);
}

int (read_KBC_output)(uint8_t port, uint8_t *output) {

    uint8_t status;
    uint8_t attemps = MAX_ATTEMPS;
    
    while (attemps) {

        if (read_KBC_status(&status) != 0) {
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & FULL_OUT_BUFFER) != 0) {

            if(util_sys_inb(port, output) != 0){
                printf("Error: Full buffer!\n");
                return 1;
            }

            if((status & PARITY_ERROR) != 0){
                printf("Error: Parity error!\n");
                return 1;
            }

            if((status & TIMEOUT_ERROR) != 0){
                printf("Error: Timeout error!\n");
                return 1;
            }

            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attemps--;
    }

    return 1;
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte) {

    uint8_t status;
    uint8_t attemps = MAX_ATTEMPS;

    while (attemps) {

        if (read_KBC_status(&status) != 0){
            printf("Error: Status not available!\n");
            return 1;
        }

        if ((status & FULL_IN_BUFFER) == 0){

            if(sys_outb(port, commandByte) != 0){
                printf("Error: Could not write commandByte!\n");
                return 1;
            }

            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBC));
        attemps--;
    }
    
    return 1;
}

void (kbc_ih)() {
    if (read_KBC_output(KBC_OUT_CMD, &scancode) != 0) printf("Error: Could not read scancode!\n");
}

int (kbc_restore)() {
    uint8_t commandByte;

    if (write_KBC_command(KBC_IN_CMD, KBC_READ_CMD) != 0) return 1;          
    if (read_KBC_output(KBC_OUT_CMD, &commandByte) != 0) return 1; 

    commandByte |= ENABLE_INT;  

    if (write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD) != 0) return 1;    
    if (write_KBC_command(KBC_WRITE_CMD, commandByte) != 0) return 1;

    if (kbd_print_no_sysinb(counter_KBC) != 0) return 1;

    return 0;
}

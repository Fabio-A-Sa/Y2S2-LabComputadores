#include "KBC.h"

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
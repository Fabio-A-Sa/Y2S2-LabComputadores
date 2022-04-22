/* 
* Timer
* Preparação para o Teste 
* @author: Fábio Araújo de Sá
*/

/* Macros */

#define BIT(n) ( 0x1 << (n) )
#define IRQ_CHANEL 1

/* Status */

int getStatus(uint8_t *result) {
    return sys_inb(KBC_STATUS_REGISTER, result);
}

/* Read ScanCode */

int readScancode(uint8_t *result) {

    int attemps = 5;
    while (attemps) {
        uint8_t status;
        getStatus(&status);
        if (status & OUTPUT_BUFFER_FULL) {
            if (!(status & PARITY_ERROR) && !(status & TIMEOUT_ERROR)) {
                if (sys_inb(PORT, result) != 0) return 1;
                return 0;
            }
        }
        tikdelay(micros_to_ticks(WAIT_X_SECONDS));
        attemps--;
    }
    return 1;
}

/* Write in KBC */

int writeKBC(uint8_t controlWord) {

    int attemps = 5;
    while (attemps) {

        uint8_t status;
        getStatus(&status);
        if (status & INPUT_BUFFER_FULL) {
            if (sys_outb(PORT, controlWord) != 0) return 1; 
            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_X_SECONDS));
        attemps--;
    }
    return 1;
}

/* Restore Interrupts */

int restoreInterrupts() {

    
}
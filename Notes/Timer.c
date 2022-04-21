/* 
* Timer
* Preparação para o Teste 
* @author: Fábio Araújo de Sá
*/

/* Macros */

#define BIT(n) ( 0x1 << (n) )
#define IRQ_CHANEL 0

/* LSB e MSB */

void getLSB(uint16_t number, uint8_t *result) {
    *result = number & 0xFF;
}

void getMSB(uint16_t number, uint8_t *result) {
    *result = (number >> 8) & 0xFF;
}

/* Interrupções */

uint8_t hook_id = 0;
int subscribe_timer(uint8_t *bit_no) {
    *bit_no = BIT(hook_id);
    return sys_iqrsetpolicy(IRQ_CHANEL, IRQ_REENABLE, &hook_id);
}

int unsubscribe_timer() {
    return sys_iqrrmpolicy(&hook_id);
}

void cicle() {

    
}
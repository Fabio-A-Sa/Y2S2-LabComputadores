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

    int ipc_status, r;
    uint8_t irq_set;
    message msg;

    if (subscribe_timer(&irq_set) != 0) return 1;
  
    while(something) {

        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) { 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:              
                    if (msg.m_notify.interrupts & irq_set) {
                        // DO SOMETHING
                    }
                    break;
                default:
                    break;    
            }
    }

    return unsubscribe_timer();
}

/* Get configuration */

int getConfig(int t, uint8_t *result) {

    uint8_t controlWord;                                // a dizer que quer ler do timer t
    uint32_t definition;
    if (sys_outb(0x43, controlWord) != 0) return 1;
    if (sys_inb(0x40, &definition) != 0) return 1;      // admitindo que t tem registo 0x40
    *result = definition & 0xFF;                        // corta os primeiros 8 bits
    return 0;
} 

/* Set Frequency */

int setFreq(int newFreq, int timer) {

    uint8_t controlWord;                                // a dizer que quer escrever MSB e LSB do timer t
    if (sys_outb(0x43, controlWord) != 0) return 1;     // injecta assim a control word
    uint16_t toWrite = MAX_FREQ / newFreq;              // escreve o valor interno máximo
    uint8_t MSB, LSB;
    getMSB(toWrite, &MSB);                              // separa o valor em duas partes
    getLSB(toWrite, &LSB);                              // a mais significativa e a menos significativa
    if (sys_outb(0x40, LSB) != 0) return 1;             // admitindo t = 0
    if (sys_outb(0x40, MSB) != 0) return 1;             // admitindo t = 0
    return 0;
}
// Resolução PP1 2020 - The L3223
// Fábio Sá 2023

#include <stdlib.h>
#include <stdint.h>

// Macros úteis
#define BIT(N)              (1 << (N))     
#define TIMER_0             0x20
#define TIMER_1             0x21
#define TIMER_2             0x22
#define CONTROL_REGISTER    0x23
#define STATUS_REGISTER     0x23

// ControlWord
#define SELECT_TIMER_0      0
#define SELECT_TIMER_1      BIT(6)
#define SELECT_TIMER_2      BIT(7)
#define ALARM_MODE          BIT(4)
#define ALARM_PERIODIC      0
#define TIMER_MICROSECONDS  0
#define TIMER_MILLISECONDS  BIT(0)
#define TIMER_SECONDS       BIT(1)

// Interrupções
#define TIMER_0_INT         BIT(0)
#define TIMER_1_INT         BIT(1)
#define TIMER_2_INT         BIT(2)
#define L3223_IRQ_LINE      10
int l3223_hook_id = 0;      // qualquer valor [0..7] serve

/*
Não declarar, já está na LCF
enum l3223_time_units {
    l3223_microsecond,
    l3223_millisecond,
    l3223_second
};
*/

int get_LSB(int value, uint8_t *lsb) {
    if (lsb == NULL) return 1;
    *lsb = value & 0xFF;
    return 0;
}

int get_MSB(int value, uint8_t *msb) {
    if (msb == NULL) return 1;
    *msb = value >> 8;
    return 0;
}

int read_status(uint8_t *status) {
    uint32_t output;
    if (sys_inb(STATUS_REGISTER, &output)) return 1;
    *status = output & 0xFF;
    return 0;
}

int l3223_subscribe_int(uint8_t *mask) {
    if (mask == NULL) return 1;
    *mask = BIT(l3223_hook_id);
    return sys_irqsetpolicy(L3223_IRQ_LINE, IRQ_REENABLE, &l3223_hook_id);
}

int l3223_unsubscribe_int() {
    return sys_irqrmpolicy(&l3223_hook_id);
}

int pp_test_alarm(int timer, int interval, enum l3223_time_units unit) {

    /* ====== Validação de inputs ====== */

    // timer 0, 1 ou 2 apenas
    if (timer > 2 || timer < 0) return 1;
    // intervalo de no máximo 16 bits, para poder ser injetado nos contadores
    if ((interval >> 16) != 0) return 1;

    /* ====== Construção da ControlWord ====== */

    // Ativar o modo alarme
    uint8_t controlWord = ALARM_MODE;
    uint8_t selectedTimer;      // é importante depois saber qual é o timer a usar
    uint8_t intTimerSelected;   // é importante depois saber qual é o bit ativo durante a interrupção

    // Seleção do timer a afetar
    switch (timer) {
        case 0:
            controlWord |= SELECT_TIMER_0;
            selectedTimer = TIMER_0;
            intTimerSelected = TIMER_0_INT;
            break;
        case 1:
            controlWord |= SELECT_TIMER_1;
            selectedTimer = TIMER_1;
            intTimerSelected = TIMER_1_INT;
            break;
        case 2:
            controlWord |= SELECT_TIMER_2;
            selectedTimer = TIMER_2;
            intTimerSelected = TIMER_2_INT;
            break;
        default:
            return 1;
    }

    // Seleção da unidade de tempo a usar
    switch (unit) {
        case l3223_microsecond:
            controlWord |= TIMER_MICROSECONDS;
            break;
        case l3223_millisecond:
            controlWord |= TIMER_MILLISECONDS;
            break;
        case l3223_second:
            controlWord |= TIMER_SECONDS;
            break;
        default:
            return 1;
    }

    /* ====== Configuração do L3223 ====== */

    // Converter o intervalo em LSB + MSB
    uint8_t lsb, msb;
    if (get_LSB(interval, &lsb)) return 1;
    if (get_MSB(interval, &msb)) return 1;

    // Mandar a nova configuração do L3223 pelo registo de controlo
    if (sys_outb(CONTROL_REGISTER, controlWord)) return 1;

    // Injetar o novo valor do contador, LSB e depois MSB, diretamente na porta do timer selecionado
    if (sys_outb(selectedTimer, lsb)) return 1;
    if (sys_outb(selectedTimer, msb)) return 1;

    /* ====== Interrupções ====== */

    // Ativar as interrupções
    uint8_t l3223_mask;
    if (l3223_subscribe_int(&l3223_mask)) return 1;

    // Ciclo while que lida com as interrupções
    int ipc_status;
    message msg;
    uint8_t done = 0;       // Critério de paragem
    uint8_t l3223_status;   // Importante para avaliarmos qual dos timers deu a interrupção
    while (!done){

        if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
            printf("Error");
            continue;
        }

        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)){
                 case HARDWARE:
                    if (msg.m_notify.interrupts & l3223_mask) {     // Caso seja uma interrupção do L3223
                        if (read_status(&l3223_status)) return 1;   // Lemos o status
                        if (l3223_status & intTimerSelected) {      // É uma interrupção do timer selecionado    
                            if (pp_print_alarm(timer, interval, unit)) return 1; // Chama a função do enunciado
                            done = 1;                               // Sai do loop na próxima iteração
                        }   
                    }
            }
        }
    }

    // Desativar as interrupções
    if (l3223_unsubscribe_int()) return 1;

    return 0;  
}
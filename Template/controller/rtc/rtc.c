#include "rtc.h"

// Variáveis globais
int rtc_hook_id = RTC_MASK; // Máscara constante = 5
real_time_info time_info;   // Estrutura que irá conter toda a informação
uint8_t binary_mode;        // Modo de contagem, variável booleana

// O setup consiste em determinar o modo de contagem do RTC
// E também preencher pela primeira vez a informação do tempo
void rtc_setup() {
    binary_mode = rtc_is_binary();
    rtc_update_time();
}

// Subscrição das interrupções. Semelhante aos labs anteriores.
int rtc_subscribe_interrupts() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

// Desativação das interrupções. Semelhante aos labs anteriores.
int rtc_unsubscribe_interrupts() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

// Leitura do output do RTC, dado um comando
int rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(REGISTER_INPUT, command) != 0) return 1;
	if (util_sys_inb(REGISTER_OUTPUT, output) != 0) return 1;
    return 0;
}

// Retorna 1 se naquele momento o RTC está a atualizar os seus valores internos
// Nesse caso não devemos ler nenhum registo
int rtc_is_updating() {
    uint8_t result;
    if (rtc_output(REGISTER_UPDATING, &result)) return 1;
	return result & UPDATING;
}

// Retorna 1 se o modo de contagem for binário
int rtc_is_binary() {
    uint8_t result;
    if (rtc_output(REGISTER_COUNTING, &result)) return 1;
	return result & BINARY;
}

// Retorna 1 se o modo de contagem for BCD
int rtc_is_bcd() {
    return !rtc_is_binary();
}

// Transforma um valor de 8 bits BCD em binário
uint8_t to_binary(uint8_t bcd_number) {
    return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

// Faz update às informações da struct time_info
int rtc_update_time() {
    
    // Se o RTC estiver ocupado a atualizar os registos não devemos ler dados
    if (rtc_is_updating() != 0) return 1;
    uint8_t output;

    // Seconds
    if (rtc_output(SECONDS, &output) != 0) return 1;
    time_info.seconds = binary_mode ? output : to_binary(output);

    // Minutes
    if (rtc_output(MINUTES, &output) != 0) return 1;
    time_info.minutes = binary_mode ? output : to_binary(output);

    // Hours
    if (rtc_output(HOURS, &output) != 0) return 1;
    time_info.hours = binary_mode ? output : to_binary(output);

    // Day
    if (rtc_output(DAY, &output) != 0) return 1;
    time_info.day = binary_mode ? output : to_binary(output);

    // Month
    if (rtc_output(MONTH, &output) != 0) return 1;
    time_info.month = binary_mode ? output : to_binary(output);

    // Year
    if (rtc_output(YEAR, &output) != 0) return 1;
    time_info.year = binary_mode ? output : to_binary(output);

    return 0;
}

#include "rtc.h"

int rtc_hook_id = RTC_MASK;
real_time_info time_info;
uint8_t binary_mode;

void rtc_setup() {
    binary_mode = rtc_is_binary();
}

int rtc_subscribe_interrupts() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id);
}

int rtc_unsubscribe_interrupts() {
    return sys_irqrmpolicy(&rtc_hook_id);
}

int rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(REGISTER_INPUT, command) != 0) return 1;
	if (util_sys_inb(REGISTER_OUTPUT, output) != 0) return 1;
    return 0;
}

int rtc_is_updating() {
    uint8_t result;
    if (rtc_output(REGISTER_UPDATING, &result)) return 1;
	return result & UPDATING;
}

int rtc_is_binary() {
    uint8_t result;
    if (rtc_output(REGISTER_COUNTING, &result)) return 1;
	return result & BINARY;
}

int rtc_is_bcd() {
    return !rtc_is_binary();
}

uint8_t to_binary(uint8_t bcd_number) {
    return (bcd_number >> 4) * 10 + bcd_number & 0xF;
}

int rtc_update_time() {
    
    // Se estiver ocupado a atualizar não devemos ler os dados
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

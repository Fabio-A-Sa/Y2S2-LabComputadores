#include "rtc.h"

real_time_info time_info;

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

int rtc_update_time() {
    
    // Se estiver ocupado a atualizar não devemos ler os dados
    if (rtc_is_updating() != 0) return 1;
    uint8_t output;

    // Seconds
    if (rtc_output(SECONDS, &output) != 0) return 1;
    time_info.seconds = output;

    // Minutes
    if (rtc_output(MINUTES, &output) != 0) return 1;
    time_info.minutes = output;

    // Hours
    if (rtc_output(HOURS, &output) != 0) return 1;
    time_info.hours = output;

    // Day
    if (rtc_output(DAY, &output) != 0) return 1;
    time_info.day = output;

    // Month
    if (rtc_output(MONTH, &output) != 0) return 1;
    time_info.month = output;

    // Year
    if (rtc_output(YEAR, &output) != 0) return 1;
    time_info.year = output;

    return 0;
}

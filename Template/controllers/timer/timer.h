#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#define TIMER_MASK BIT(0)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

int (timer_subscribe_interrupts)();

int (timer_unsubscribe_interrupts)();

int (timer_get_conf)(uint8_t timer, uint8_t *st);

#endif

#ifndef _LCOM_SERIAL_PORT_H_
#define _LCOM_SERIAL_PORT_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define IRQ_SERIAL 4
#define SERIAL_MASK BIT(4)

int serial_subscribe_interrupts();
int serial_unsubscribe_interrupts();

#endif

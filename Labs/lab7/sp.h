#ifndef _LCOM_SP_H_
#define _LCOM_SP_H_

#define SP_MASK         BIT(6)

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "UART.h"
#include "queue.h"

int (sp_setup)();
int (sp_subscribe_interrupts)(uint8_t *bit_no);
int (sp_unsubscribe_interrupts)();
void (sp_ih)();
int (read_sp_data)();
int (read_lsr)(uint8_t *lsr);
int (write_sp_data)(uint8_t data);
uint8_t (prepare_move_byte)(uint8_t position, uint32_t color, uint8_t remove);

#endif

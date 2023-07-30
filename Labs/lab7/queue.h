#ifndef _LCOM_QUEUE_H_
#define _LCOM_QUEUE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "sp.h"

int push(uint8_t byte);
uint8_t pop();
bool has_byte();
int update_queue();
int retry();

#endif

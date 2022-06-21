#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

int (mouse_subscribe)(uint8_t *bit_no);
void (mouse_ih)();
int (mouse_unsubscribe)();

#endif

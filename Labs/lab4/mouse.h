#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (mouse_sync_bytes)();

void (mouse_bytes_to_packet)();

int (mouse_write)(uint8_t command);

#endif

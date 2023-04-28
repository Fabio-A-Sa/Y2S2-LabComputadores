#ifndef _LCOM_PROTOCOL_H_
#define _LCOM_PROTOCOL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "queue.h"
#include "port.h"

int init_protocol();
int handshake();
int exit_protocol();

#endif

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_MOUSE 12

#define KBC_DATA_PORT 0x60
#define KBC_STATUS_REG 0x64
#define KBC_WRITE_CMD 0x64
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60

#define DISABLE_MOUSE 0xA7
#define ENABLE_MOUSE 0xA8
#define WRITE_BYTE_MOUSE 0xD4

#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define AUX BIT(5)  //1 if data from mouse
#define INH BIT(4) //0 if keyboard is inhibited
#define FULL_IN_BUFFER BIT(1)
#define FULL_OUT_BUFFER BIT(0)




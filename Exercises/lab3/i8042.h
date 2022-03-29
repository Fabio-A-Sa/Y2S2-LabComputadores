#ifndef I8042_H
#define I8042_H

#define BIT(n)          (0x01 << (n))

#define IRQ_KBC         1

#define ESC		        0x81

#define TIMEOUT_ERR     BIT(6)
#define PARITY_ERR      BIT(7)

#define KBC_ST_REG      0x64
#define KBC_CMD_REG     0x64
#define KBC_OUT_BUF     0x60
#define KBC_IN_BUF     0x60

#define TWO_BYTES 		0xE0

#define OBF             BIT(0)
#define IBF             BIT(1)

#define WAIT_KBC 		20000
#define MAX_BYTES       2

#endif /* I8042_H */
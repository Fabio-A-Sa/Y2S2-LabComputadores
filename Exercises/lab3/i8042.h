#ifndef I8042_H
#define I8042_H

#define KBD_IRQ  1

#define ESC_BREAK 		0x81

#define STAT_REG 		0x64
#define KBC_CMD_REG     0x64
#define IN_BUF		    0x64
#define OUT_BUF 		0x60
#define OUT_BUF_FULL 0x01

#define BIT(n)    (0x01<<(n))
#define TWO_BYTES 		0xe0

#define OBF            BIT(0)
#define IBF            BIT(1)

#define WAIT_KBC 		20000

#define PAR_ERR BIT(7)
#define TO_ERR  BIT(6)

#define LEDS_CMD 		0xED
#define ACK 			0xFA
#define RESEND 			0xFE
#define ERROR 			0xFC

#define TRIES 			5


#endif /* I8042_H */
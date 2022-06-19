#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* IRQ Lines */

#define IRQ_KBC         1
#define IRQ_MOUSE       12

/* KBC */

#define MAKE_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER  BIT(1)

#define KBC_STATUS_REG  0x64
#define KBC_IN_CMD      0x64
#define KBC_OUT_CMD     0x60
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

/* Mouse */

#define ENABLE_INT      BIT(0)
#define MOUSE_TICK      BIT(5)
#define MOUSE_SYNC      BIT(3)
#define MOUSE_IN        0xD4
#define MOUSE_ACK       0xFA
#define MOUSE_NACK      0xFE
#define MOUSE_ERROR     0xFC
#define MOUSE_LB        BIT(0)
#define MOUSE_RB        BIT(1)
#define MOUSE_MB        BIT(2)
#define MOUSE_X_DELTA   BIT(4)
#define MOUSE_Y_DELTA   BIT(5)
#define MOUSE_X_SIGNAL  BIT(6)
#define MOUSE_Y_SIGNAL  BIT(7)

#define WAIT_KBC        20000
#define MAX_ATTEMPS     10

/* Scancodes */

#define MAKE_ESC        0x01 
#define BREAK_ESC       0x81
#define MAKE_ENTER      0x1C
#define BREAK_ENTER     0x9C
#define MAKE_MINUS      0x0C
#define MAKE_PLUS       0x0D
#define MAKE_BACKSPACE  0x0E

#define MAKE_1          0x02
#define MAKE_2          0x03
#define MAKE_3          0x04
#define MAKE_4          0x05
#define MAKE_5          0x06
#define MAKE_6          0x07
#define MAKE_7          0x08
#define MAKE_8          0x09

#define MAKE_A          0x1E
#define MAKE_B          0x30
#define MAKE_C          0x2E	
#define MAKE_D          0x20
#define MAKE_E          0x12
#define MAKE_F          0x21
#define MAKE_G          0x22
#define MAKE_H          0x23
#define MAKE_I          0x17
#define MAKE_J          0x24
#define MAKE_K          0x25
#define MAKE_L          0x26
#define MAKE_M          0x32
#define MAKE_N          0x31
#define MAKE_O          0x18
#define MAKE_P          0x19
#define MAKE_Q          0x10
#define MAKE_R          0x13
#define MAKE_S          0x1F
#define MAKE_T          0x14
#define MAKE_U          0x16
#define MAKE_V          0x2F
#define MAKE_W          0x11
#define MAKE_X          0x2D
#define MAKE_Y          0x15
#define MAKE_Z          0x2C

#endif /* _LCOM_I8042_H_ */

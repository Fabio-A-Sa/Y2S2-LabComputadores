#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define TIMER      0
#define TIMER_FREQ 1193182 
#define TIMER0_IRQ 0

#define TIMER_0    0x40
#define TIMER_1    0x41
#define TIMER_2    0x42
#define TIMER_CTRL 0x43

#define SPEAKER_CTRL 0x61 

#define TIMER_SEL0   0x00             
#define TIMER_SEL1   BIT(6)         
#define TIMER_SEL2   BIT(7)           
#define TIMER_RB_CMD (BIT(7) | BIT(6))

#define TIMER_LSB     BIT(4)                
#define TIMER_MSB     BIT(5)                
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) 
#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) 
#define TIMER_RATE_GEN BIT(2)        

#define TIMER_BCD 0x01
#define TIMER_BIN 0x00
#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

#endif

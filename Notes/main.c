// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/pp.h>

#define BIT(n) (0x1 << (n))

#define TIMER_0 0x20
#define TIMER_1 0x21
#define TIMER_2 0x22
#define TIMER_CTRL 0x23

#define TIMER_SEL0 0x00
#define TIMER_SEL1 BIT(6)
#define TIMER_SEL2 BIT(7)

#define PERIODIC_MODE 0x00
#define ALARM_MODE BIT(4)

#define TIMER_MICROSEC 0x00
#define TIMER_MILLISEC BIT(0)
#define TIMER_SECONDS BIT(1)

#define TIMER2_INTERRUPT BIT(2)
#define TIMER1_INTERRUPT BIT(1)
#define TIMER0_INTERRUPT BIT(0) 

#include <stdint.h>

#define IRQ_LINE = 10;
int hook_id = 0;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/pp/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/pp/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int pp_test_alarm(int timer, int interval, enum l3223_time_units unit) {

    if (timer < 0 || timer > 2 || interval <= 0) return 1;

    uint8_t controlWord = 0x00 | ALARM_MODE;
    
    uint8_t selectedTimer, selTimerInterrupt;
    switch(timer){
        case 0: 
            controlWord = controlWord | TIMER_SEL0;
            selectedTimer = TIMER_0;
            selTimerInterrupt = TIMER0_INTERRUPT;
            break;
        case 1:
            controlWord = controlWord | TIMER_SEL1;
            selectedTimer = TIMER_1;
            selTimerInterrupt = TIMER1_INTERRUPT;
            break;
        case 2:
            controlWord = controlWord | TIMER_SEL2;
            selectedTimer = TIMER_2;
            selTimerInterrupt = TIMER2_INTERRUPT;
            break;
        default: return 1;
    }

    switch(unit){
        case l3223_microsecond:
            controlWord = controlWord | TIMER_MICROSEC;
            break;
        case l3223_millisecond:
            controlWord = controlWord | TIMER_MILLSEC;
            break;
        case l3223_second:
            controlWord = controlWord | TIMER_SECONDS;
            break;
        default: 
            return 1;
    }

    uint8_t LSB, MSB;
    LSB = 0xff & interval;
    MSB = 0xff & (interval >> 8);

    if (sys_outb(TIMER_CTRL, controlWord) != 0) return 1;
    if (sys_outb(selectedTimer,LSB) != 0) return 1;
    if (sys_outb(selectedTimer,MSB) != 0) return 1;
 
  int ipc_status, r, irq_set = BIT(hook_id);
  message msg;

  bool done = false;

  // Subscribe interrupts
  if (sys_irqsetpolicy(IRQ_LINE, IRQ_REENABLE, &hook_id) != 0) return 1;

  // interrupt loop
  while (!done) {
    // Get a request message.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                             // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
                                                   // handle interrupt
                uint32_t timerReadBack;
                if( sys_inb(TIMER_CTRL, &timerReadBack) != 0) return 1;

                if( (uint8_t)timerReadBack & selTimerInterrupt){
                    pp_print_alarm(timer, interval, unit);
                    done = true;
                }
          }
          break;
        default:
          break;
      }
    }
  }

  // Unsubscribe interrupts
    if( sys_irqrmpolicy(&hook_id) != 0) return 1;

  return 0;
}
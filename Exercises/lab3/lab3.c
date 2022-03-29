#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

extern int counter;         // KBC.h counter
extern uint8_t scancode;    // KBC.c scancode -> é atualizado por cada chamada de kcb_in() se o buffer de output estiver cheio

int main(int argc, char *argv[]) {

  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {

    int index = 0;
    uint8_t scancode;
    uint8_t irq_set;
    int ipc_status;
    message msg;
    uint8_t content[MAX_BYTES];

    if (subscribe_KBC_interrupts(&irq_set) != 0) return 1;

    while(scancode != ESC) { /* Run while ESC key isn't pressed */

        /* Get a request message */
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        /* Tratamento do interrupt caso seja uma notificação */
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set) { /* subscribed keyboard interrupt */

                        kbc_ih(); /* handler keyboard interrupts -> read data e atualiza scancode */

                        if (scancode == TWO_BYTES) {        // se for para ler 2 bytes
                            content[index] = scancode;      // coloca o LSB e deixa espaço para o MSB, por ordem
                            index++;                        // aponta para MSB
                        }

                        content[index] = scancode;                                      // coloca o restante, se necessário
                        kbd_print_scancode(!(scancode & BIT(7)), index+1, content);     // chama a função dos profs para printar o conteúdo
                        index = 0;                                                      // volta ao início do array

                    } break;

                default:
                    break; /* no other notifications expected */
            }
        }
    }

    if (unsubscribe_KBC_interrupts() != 0) return 1;
    if (kbd_print_no_sysinb(counter) != 0) return 1;

    return 0;
}

int(kbd_test_poll)() {

int index = 0;
    uint8_t scancode;
    uint8_t irq_set;
    int ipc_status;
    message msg;
    uint8_t content[MAX_BYTES];

    if (subscribe_KBC_interrupts(&irq_set) != 0) return 1;

    while(scancode != ESC) { /* Run while ESC key isn't pressed */

        /* Get a request message */
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        /* Tratamento do interrupt caso seja uma notificação */
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE: 
                    if (msg.m_notify.interrupts & irq_set) { /* subscribed keyboard interrupt */

                        kbc_ih(); /* handler keyboard interrupts -> read data e atualiza scancode */

                        if (scancode == TWO_BYTES) {        // se for para ler 2 bytes
                            content[index] = scancode;      // coloca o LSB e deixa espaço para o MSB, por ordem
                            index++;                        // aponta para MSB
                        }

                        content[index] = scancode;                                      // coloca o restante, se necessário
                        kbd_print_scancode(!(scancode & BIT(7)), index+1, content);     // chama a função dos profs para printar o conteúdo
                        index = 0;                                                      // volta ao início do array

                    } break;

                default:
                    break; /* no other notifications expected */
            }
        }
    }

    if (unsubscribe_KBC_interrupts() != 0) return 1;
    if (kbd_print_no_sysinb(counter) != 0) return 1;

    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {


    return 1;
}
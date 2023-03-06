// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "KBC.c"
#include "mouse.h"

extern struct packet mouse_packet;
extern uint8_t byte_index;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
   
  int ipc_status;
  message msg;
  uint8_t mouse_mask;

  if (mouse_subscribe_int(&mouse_mask) != 0) return 1;
  if (my_solution(0xEA) != 0) return 1; // set stream mode
  if (my_solution(0xF4) != 0) return 1; // enable data report
  //if (mouse_enable_data_reporting() != 0) return 1;

  while (cnt){

    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 
          if (msg.m_notify.interrupts & mouse_mask){
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index == 3) {
              mouse_bytes_to_packet();
              mouse_print_packet(&mouse_packet);
              byte_index = 0;
              cnt--;
            }
          }
          break;
      }
    }
  }
  
  if (mouse_unsubscribe_int() != 0) return 1;
  if (my_solution(0xFF) != 0) return 1; // rm stream mode
  //if (my_solution(0xF5) != 0) return 1; // disable data report
  return 0;
}

/*
int (mouse_test_async)(uint8_t idle_time) {
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)() {
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
*/

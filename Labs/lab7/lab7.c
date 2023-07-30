#include <lcom/lcf.h>
#include <string.h>
#include <stdio.h>
#include <queue.h>
#include <sp.h>

int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/lab7/debug/trace.txt");
  lcf_log_output("/home/lcom/labs/lab7/debug/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

    if (argc != 1 || (strcmp(argv[0], "0") != 0 && strcmp(argv[0], "1") != 0)) {
        printf("usage: lcom_run proj <TARGET>\n");
        printf("       TARGET: 0 - Transmitter\n");
        printf("               1 - Receiver\n");
        return 1;
    }

    uint8_t sp_mask;
    uint8_t target = atoi(argv[0]);
    if (sp_setup() != 0) {
        printf("SP setup failed\n");
        return 1;
    }

    if (sp_subscribe_interrupts(&sp_mask) != 0) {
        printf("SP subscribe failed\n");
        return 1;
    }


    if (target == 0) {
        
        printf("SP transmitter\n");

        uint8_t ch;
        while (1) {

            ch = getchar(); // Read character from user input
            
            // Check if the user wants to quit
            if (ch == 'q' || ch == 'Q') {
                break;
            }
            
            if (write_sp_data('f') != 0) break; // Call the function with the character
        }
    }

    if (target == 1) {

        printf("SP receiver\n");
        
        int ipc_status;
        message msg;
        while (1) {
            
            if (driver_receive(ANY, &msg, &ipc_status) != 0) {
                printf("Error");
                continue;
            }

            if (is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 
                    if (msg.m_notify.interrupts & sp_mask) {
                        sp_ih();
                        printf("Data received: f\n");
                    }
            }
            }
        }
    }
  

    if (sp_unsubscribe_interrupts() != 0) return 1;

    return 0;
}

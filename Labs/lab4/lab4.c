#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "i8254.h"
#include "KBC.c"
#include "mouse.h"
#include "timer.c"

extern struct packet mouse_packet;
extern uint8_t byte_index;
extern int timer_counter;

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
  uint8_t mouse_mask; // Para interpretar as interrupções

  // Subscrição das interrupções do rato
  if (mouse_subscribe_int(&mouse_mask) != 0) return 1;

  // Ativar o report de dados do rato com
  // A -> Função implementada de raíz
  // B -> Função dada pelos professores
  if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1; // A
  //if (mouse_enable_data_reporting() != 0) return 1; // B

  while (cnt) { // Só termina quando lermos @cnt pacotes

    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 
          if (msg.m_notify.interrupts & mouse_mask){  // Se for uma interrupção do rato
            mouse_ih();                               // Lemos mais um byte
            mouse_sync_bytes();                       // Sincronizamos esse byte no pacote respectivo
            if (byte_index == 3) {                    // Quando tivermos três bytes do mesmo pacote
              mouse_bytes_to_packet();                // Formamos o pacote
              mouse_print_packet(&mouse_packet);      // Mostramos o pacote
              byte_index = 0;
              cnt--;
            }
          }
          break;
      }
    }
  }
  
  // Desativar o report de dados do rato
  if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

  // Desativar as interrupções
  if (mouse_unsubscribe_int() != 0) return 1;
 
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    
    // A LCF já lança o rato em REMOTE MODE / Polling

    while (cnt) { // Só termina quando lermos @cnt pacotes

        if (mouse_write(MOUSE_READ_DATA) != 0) return 1;  // Comando para lermos diretamente mais um byte
        mouse_ih();                                       // Lemos mais um byte    
        mouse_sync_bytes();                               // Sincronizamos esse byte no pacote respectivo      
        if (byte_index == 3) {                            // Quando tivermos três bytes do mesmo pacote
            mouse_bytes_to_packet();                      // Formamos o pacote
            mouse_print_packet(&mouse_packet);            // Mostramos o pacote 
            byte_index = 0;
            cnt--;
            tickdelay(micros_to_ticks(period * 1000));    // Esperamos @period microssegundos até ler o próximo pacote
        }
    }

    // Ativar o Stream Mode
    if (mouse_write(ENABLE_STREAM_MODE) != 0) return 1;

    // Desativar o report de dados do rato
    if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;
  
    // KBC volta ao estado normal
    uint8_t commandByte = minix_get_dflt_kbc_cmd_byte();              // buscar o comando adequado, já dado pela LCF
    if (write_KBC_command(KBC_IN_CMD, KBC_WRITE_CMD) != 0) return 1;  // avisar o i8042 para receber o próximo comando
    if (write_KBC_command(KBC_WRITE_CMD, commandByte) != 0) return 1; // enviar o comando
  
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {

  int ipc_status;
  message msg;
  uint8_t seconds = 0;
  uint8_t mouse_mask = 0, timer_mask = 0; // Para interpretar as interrupções

  // Subscrição das interrupções
  if (mouse_subscribe_int(&mouse_mask) != 0) return 1;
  if (timer_subscribe_int(&timer_mask) != 0) return 1;

  // Ativar o report de dados do rato com
  // A -> Função implementada de raíz
  // B -> Função dada pelos professores
  if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1; // A
  //if (mouse_enable_data_reporting() != 0) return 1; // B

  while (seconds < idle_time) { // Só termina quando passarmos @idle_time sem ler pacotes

    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE: 

          if (msg.m_notify.interrupts & timer_mask) { // Se for uma interrupão do timer
            timer_int_handler();
            if (timer_counter % 60 == 0) seconds++;
          }

          if (msg.m_notify.interrupts & mouse_mask){  // Se for uma interrupção do rato
            mouse_ih();                               // Lemos mais um byte
            mouse_sync_bytes();                       // Sincronizamos esse byte no pacote respectivo
            if (byte_index == 3) {                    // Quando tivermos três bytes do mesmo pacote
              mouse_bytes_to_packet();                // Formamos o pacote
              mouse_print_packet(&mouse_packet);      // Mostramos o pacote
              byte_index = 0;
            }
            seconds = 0;
            timer_counter = 0;
          }
      }
    }
  }

  // Desativar o report de dados do rato
  if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

  // Desativar as interrupções
  if (timer_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

/*
int (mouse_test_gesture)() {
    printf("%s: under construction\n", __func__);
    return 1;
}

*/

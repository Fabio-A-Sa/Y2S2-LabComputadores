#include <lcom/lcf.h>
#include "controllers/timer/timer.h"
#include "controllers/video/graphics.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/mouse/mouse.h"
#include "states/states.h"
#include "config.h"

extern SystemState systemState;

int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/Template/trace.txt");
  lcf_log_output("/home/lcom/labs/Template/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

int setup() {

  // Atualização da frequência
  if (timer_set_frequency(0, GAME_FREQUENCY) != 0) return 1;

  // Inicialização do modo gráfico
  if (set_graphic_mode(VIDEO_MODE) != 0) return 1;

  // Ativação das interrupções dos dispositivos
  if (timer_subscribe_interrupts() != 0) return 1;
  if (keyboard_subscribe_interrupts() != 0) return 1;
  if (mouse_subscribe_interrupts() != 0) return 1;

  // Ativar stream-mode e report de dados do rato
  if (mouse_write(ENABLE_STREAM_MODE) != 0) return 1;
  if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;

  return 0;
}

int teardown() {

  // Volta ao modo de texto
  if (vg_exit() != 0) return 1;

  // Desativa todas as interrupções
  if (timer_unsubscribe_interrupts() != 0) return 1;
  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  if (mouse_unsubscribe_interrupts() != 0) return 1;

  // Desativar o report de dados do rato
  if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

  // Setup do Minix
  if (setup() != 0) return teardown();

  // Tratamento das Interrupções
  int ipc_status;
  message msg;
  while (systemState == RUNNING) {
    
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & TIMER_MASK)    update_timer_state();
          if (msg.m_notify.interrupts & KEYBOARD_MASK) update_keyboard_state();
          if (msg.m_notify.interrupts & MOUSE_MASK)    update_mouse_state();
        }
    }
  }
  
  // Tear-down do Minix
  if (teardown() != 0) return 1;

  return 0;
}

#include <lcom/lcf.h>

#include "graphics.h"
#include "keyboard.h"
#include "timer.c"

extern vbe_mode_info_t mode_info;
extern uint8_t scancode;
extern int counter;

int main(int argc, char *argv[]) {

  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (waiting_ESC_key)() {

  int ipc_status;
  message msg;
  uint8_t keyboard_mask;

  if (keyboard_subscribe_interrupts(&keyboard_mask) != 0) return 1;

  while (scancode != BREAK_ESC){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & keyboard_mask) 
            kbc_ih();
            break;
        default:
          break; 
      }
    }
  }

  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  // Mudança para o modo gráfico
  if (set_graphic_mode(mode) != 0) return 1;

  // Espera @delay segundos
  sleep(delay);

  // Volta ao modo de texto antes de terminar
  // vg_exit() é dada pela LCF. Ver set_text_mode() para consultar a implementação interna
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  // Construção do frame buffer virtual e físico
  if (set_frame_buffer(mode) != 0) return 1;

  // Mudança para o modo gráfico
  if (set_graphic_mode(mode) != 0) return 1;

  // Desenha o rectângulo
  if (vg_draw_rectangle(x, y, width, height, color) != 0) return 1;

  // Função que retorna apenas quando ESC é pressionado
  if (waiting_ESC_key() != 0) return 1;

  // De regresso ao modo texto
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  // Construção do frame buffer virtual e físico
  if (set_frame_buffer(mode) != 0) return 1;

  // Mudança para o modo gráfico
  if (set_graphic_mode(mode) != 0) return 1;

  // Cálculo do número inteiro de rectângulos em cada eixo
  int vertical = mode_info.YResolution / no_rectangles;
  int horizontal = mode_info.XResolution / no_rectangles;

  for (int i = 0 ; i < no_rectangles ; i++) {
    for (int j = 0 ; j < no_rectangles ; j++) {

      uint32_t color;

      if (mode_info.MemoryModel == DIRECT_COLOR) {
        uint32_t R = Red(j, step, first);
        uint32_t G = Green(i, step, first);
        uint32_t B = Blue(j, i, step, first);
        color = direct_mode(R, G, B);

      } else {
        color = indexed_mode(j, i, step, first, no_rectangles);
      }

      if (vg_draw_rectangle(j * horizontal, i * vertical, horizontal, vertical, color)) return 1;
    }
  }
  
  // Função que retorna apenas quando ESC é pressionado
  if (waiting_ESC_key()) return 1;

  // De regresso ao modo texto
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  // Construção do frame buffer virtual e físico (só permite modo indexado)
  if(set_frame_buffer(VBE_768p_INDEXED)) return 1;
  
  // Mudança para o modo gráfico (só permite modo indexado)
  if(set_graphic_mode(VBE_768p_INDEXED)) return 1;

  // Imprime o XMP selecionado
  if (print_xpm(xpm, x, y) != 0) return 1;

  // Função que retorna apenas quando ESC é pressionado
  if (waiting_ESC_key()) return 1;

  // De regresso ao modo texto
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  int ipc_status;
  message msg;
  uint8_t irq_set_TIMER, irq_set_KBC; // máscaras para as interrupções

  // o movimento só pode ser horizontal ou vertical
  uint8_t vertical_direction;
  if (xi == xf && yi < yf) vertical_direction = 1;
  else if (yi == yf && xi < xf) vertical_direction = 0;
  else return 1;

  // Subscrição das interrupções
  if (keyboard_subscribe_interrupts(&irq_set_KBC) != 0) return 1;
  if (timer_subscribe_int(&irq_set_TIMER) != 0) return 1;

  // Atualiza o sistema para a frame rate dada
  if (timer_set_frequency(0, fr_rate) != 0) return 1;   

  // Construção do frame buffer virtual e físico (só permite modo indexado)
  if(set_frame_buffer(VBE_768p_INDEXED)) return 1;

  // Mudança para o modo gráfico (só permite modo indexado)
  if(set_graphic_mode(VBE_768p_INDEXED)) return 1; 

  // Imprime a imagem na posição inicial
  if (print_xpm(xpm, xi, yi) != 0) return 1;

  while (scancode != BREAK_ESC && (xi < xf || yi < yf)) {

      if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
          printf("Error");
          continue;
      }

      if(is_ipc_notify(ipc_status)) {
          switch(_ENDPOINT_P(msg.m_source)){
              case HARDWARE:

                // interrupção do teclado
                if (msg.m_notify.interrupts & irq_set_KBC) {
                    kbc_ih(); // atualiza o scancode
                }

                // interrupção do timer
                if (msg.m_notify.interrupts & irq_set_TIMER) {

                    // Apaga o desenho anterior para dar a sensação de movimetno
                    if (vg_draw_rectangle(xi, yi, 100, 100, 0xFFFFFF) != 0) return 1;

                    // Atualiza o valor da variável a incrementar
                    if (vertical_direction) {
                        yi += speed;
                        if (yi > yf) yi = yf;
                    } else {
                        xi += speed;
                        if (xi > xf) xi = xf;
                    }

                    // Imprime a nova imagem ligeiramente deslocada
                    if (print_xpm(xpm, xi, yi) != 0) return 1;
                }
          }
      }
  }

  // De regresso ao modo texto
  if (vg_exit() != 0) return 1;

  // Desativar as interrupções
  if (timer_unsubscribe_int() != 0) return 1;
  if (keyboard_unsubscribe_interrupts() != 0) return 1;

  return 0;
}

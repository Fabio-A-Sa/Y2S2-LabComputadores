#include <lcom/lcf.h>

#include "graphics.h"
#include "KBC.h"
#include "timer.c"

extern uint8_t scancode;
extern vbe_mode_info_t info;
extern int counter;

// Any header files included below this line should have been created by you

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
  uint8_t irq_set;

  if (subscribe_KBC_interrupts(&irq_set) != 0) return 1;

  while (scancode != BREAK_ESC){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & irq_set) kbc_ih();
            break;
        default:
          break; 
      }
    }
  }

  if (unsubscribe_KBC_interrupts() != 0) return 1;
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if(init_graphics_mode(mode)) return 1;
  sleep(delay);
  if(vg_exit()) return 1;
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
  if(map_vmem(mode)) return 1;
  if(init_graphics_mode(mode)) return 1;
  if(vg_draw_rectangle(x, y, width, height, color)) return 1;

  if (waiting_ESC_key()) return 1;
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  if (map_vmem(mode)) return 1;
  if (init_graphics_mode(mode)) return 1;

  int vertical = info.YResolution / no_rectangles;    // height
  int horizontal = info.XResolution / no_rectangles;  // width

  for (int i = 0 ; i < no_rectangles ; i++) {
    for (int j = 0 ; j < no_rectangles ; j++) {

      uint32_t color;

      if (info.MemoryModel == DIRECT_COLOR) {
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
  
  if (waiting_ESC_key()) return 1;
  if (vg_exit() != 0) return 1;
  return 0;
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  xpm_load(xpm, img.type, &img);

  for (int h = 0 ; h < img.height ; h++) {
    for (int w = 0 ; w < img.width ; w++) {
      if (vg_draw_pixel(x + w, y + h, *colors) != 0) return 1;
      colors++; // next color
    }
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  if(map_vmem(VBE_768p_INDEXED)) return 1;
  if(init_graphics_mode(VBE_768p_INDEXED)) return 1;

  // https://web.fe.up.pt/~pfs/aulas/lcom2122/labs/lab5/src/doc/group__xpm.html#ga069eaae77c9b41a9a04ea81666119493

  print_xpm(xpm, x, y);

  if (waiting_ESC_key()) return 1;
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

  int ipc_status;
  uint8_t irq_set_TIMER, irq_set_KBC;
  message msg;

  if(map_vmem(VBE_768p_INDEXED)) return 1;
  if(init_graphics_mode(VBE_768p_INDEXED)) return 1;

  if (subscribe_KBC_interrupts(&irq_set_KBC) != 0) return 1;
  if (timer_subscribe_int(&irq_set_TIMER) != 0) return 1;
  if (timer_set_frequency(0, fr_rate) != 0) return 1;       // atualiza para a frame rate dada

  print_xpm(xpm, xi, yi);

  while (scancode != BREAK_ESC && xi < xf) {

      if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
          printf("Error");
          continue;
      }

      if(is_ipc_notify(ipc_status)) {
          switch(_ENDPOINT_P(msg.m_source)){
              case HARDWARE:
                if (msg.m_notify.interrupts & irq_set_KBC) {
                    kbc_ih();
                }
                if (msg.m_notify.interrupts & irq_set_TIMER) {
                    vg_draw_rectangle(xi, yi, xi + speed, yi, 0xffffff);
                    xi += speed;
                    if (xi > xf) xi = xf;
                    print_xpm(xpm, xi, yi);
                } break;
              default: 
                break;
          }
      }
  }

  if (vg_exit() != 0) return 1;
  if (timer_unsubscribe_int() != 0) return 1;
  if (unsubscribe_KBC_interrupts() != 0) return 1;

  return 0;
}

#include "mouse.h"

// Variáveis globais do módulo
int hook_id_mouse = 2;        // um valor qualquer [0..7], desde que seja diferente do teclado e do timer
uint8_t byte_index = 0;       // [0..2]
uint8_t mouse_bytes[3];       // bytes apanhados
uint8_t current_byte;         // o byte mais recente lido
MouseInfo mouse_info = {0, 0, 100, 100};
extern vbe_mode_info_t mode_info;

// Subscrição das interrupções
// Modo REENABLE e modo EXCLUSIVE
int (mouse_subscribe_interrupts)(){
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

// Desativação das interrupções
int (mouse_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id_mouse);
}

// Em cada chamada do IH (interrupt handler), lê um novo byte do rato
void (mouse_ih)(){
  read_KBC_output(KBC_WRITE_CMD, &current_byte, 1);
}

// Avalia a disposição dos bytes no array @mouse_bytes
// O primeiro byte do pacote é sempre o que tem o BIT(3) ativo
void mouse_sync_bytes() {
  if (byte_index == 0 && (current_byte & FIRST_BYTE)) { // é o byte CONTROL, o bit 3 está ativo
    mouse_bytes[byte_index]= current_byte;
    byte_index++;
  }
  else if (byte_index > 0) {                            // recebe os deslocamentos em X e Y
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
}

// Transforma o array de bytes numa struct definida de acordo com as necessidades da aplicação
void (mouse_sync_info)(){

  mouse_info.right_click = mouse_bytes[0] & MOUSE_RB;
  mouse_info.left_click = mouse_bytes[0] & MOUSE_LB;

  if (mouse_bytes[0] & MOUSE_X_OVERFLOW || mouse_bytes[0] & MOUSE_Y_OVERFLOW) return;

  int16_t delta_x = mouse_info.x + ((mouse_bytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1]);
  int16_t delta_y = mouse_info.y - ((mouse_bytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2]);

  if (delta_x < 0 || delta_x > mode_info.XResolution || delta_y < 0 || delta_y > mode_info.YResolution) return;
  mouse_info.x = delta_x;
  mouse_info.y = delta_y;
}

// A escrita para o rato tem de ser feita de forma mais controlada do que no keyboard
// Temos de injetar o comando DIRETAMENTE no rato e esperar por uma resposta afirmativa (ACK).
int (mouse_write)(uint8_t command) {

  uint8_t attemps = MAX_ATTEMPS;
  uint8_t mouse_response;

  do {
    attemps--;
    if (write_KBC_command(KBC_IN_CMD, WRITE_BYTE_MOUSE)) return 1;
    if (write_KBC_command(KBC_OUT_CMD, command)) return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (util_sys_inb(KBC_OUT_CMD, &mouse_response)) return 1;
  } while (mouse_response != ACK && attemps);

  return 0;
}

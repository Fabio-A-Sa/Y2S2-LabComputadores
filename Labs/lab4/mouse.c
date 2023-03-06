#include "mouse.h"

int hook_id_mouse = 3;
struct packet mouse_packet;
uint8_t byte_index = 0;
uint8_t mouse_bytes[3];
uint8_t current_byte;

int (mouse_subscribe_int)(uint8_t *bit_no){
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id_mouse);
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
}

int (mouse_unsubscribe_int)(){
  return sys_irqrmpolicy(&hook_id_mouse);
}

void (mouse_ih)(){
  if(read_KBC_output(KBC_WRITE_CMD, &current_byte)) printf("Error in reading byte from mouse\n");
}

void (mouse_sync_bytes)(){
  if(byte_index == 0 && (current_byte & FIRST_BYTE) ){
    mouse_bytes[byte_index]= current_byte;
    byte_index++;
  }
  else{
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
}

void (mouse_bytes_to_packet)(){

  for (int i = 0 ; i < 3 ; i++) {
    mouse_packet.bytes[i] = mouse_bytes[i];
  }

  mouse_packet.lb = mouse_bytes[0] & MOUSE_LB;
  mouse_packet.mb = mouse_bytes[0] & MOUSE_MB;
  mouse_packet.rb = mouse_bytes[0] & MOUSE_RB;
  mouse_packet.x_ov = mouse_bytes[0] & MOUSE_X_OVERFLOW;
  mouse_packet.y_ov = mouse_bytes[0] & MOUSE_Y_OVERFLOW;
  mouse_packet.delta_x = (mouse_bytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1];
  mouse_packet.delta_y = (mouse_bytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2];
}

// substituta da mouse_enable_data_reporting() da LCF
int (mouse_config)(uint8_t controlWord){
  uint8_t something = 0xff, something_else = 0xff; //???? -> rever esta coisa
  uint8_t mouseResponse;
  do{
    if(write_KBC_command(KBC_IN_CMD, something)) return 1;
    if(write_KBC_command(KBC_OUT_CMD,controlWord)) return 1;
    tickdelay(micros_to_ticks(20000));
    if(read_KBC_output(KBC_OUT_CMD, &mouseResponse)) return 1;
  } while (mouseResponse != something_else);
  return 0;
}

// to enable stream mode
int (my_solution)(uint8_t command) {
  uint8_t attemps = 10;
  uint8_t mouse_response;
  do {
    attemps--;
    if (write_KBC_command(0x64, 0xD4)) return 1;
    if (write_KBC_command(0x60, command)) return 1;
    tickdelay(micros_to_ticks(WAIT_KBC));
    if (read_KBC_output(0x60, &mouse_response)) return 1;
  } while (mouse_response != ACK && attemps);
  return 0;
}

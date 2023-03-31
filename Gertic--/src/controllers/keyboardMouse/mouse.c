#include "mouse.h"

int hook_id_mouse = 4;
uint8_t mouseBytes[3];
int mouse_counter = 0;
uint32_t byte;
struct packet mouseP;
int x = 5;
int y = 5;

int (mouse_config)(uint8_t controlWord){

  uint32_t mouseResponse;

  do{
    if(KBCWrite(KBC_IN_CMD,MOUSE_IN)) return 1;
    if(KBCWrite(KBC_OUT_CMD,controlWord)) return 1;
    tickdelay(micros_to_ticks(20000));
    if(sys_inb(KBC_OUT_CMD, &mouseResponse)) return 1;

  } while (mouseResponse != MOUSE_ACK);

  return 0;
}

int (KBCWrite)(uint8_t port, uint8_t controlWord){

  uint32_t status;
  int tries = MAX_ATTEMPS;

  while(tries){
    tries--;
    if(sys_inb(KBC_IN_CMD, &status)) continue;

    while((status & FULL_IN_BUFFER)) {};

    if( status & ( TIMEOUT_ERROR | PARITY_ERROR)) return 1;

    if(sys_outb(port, controlWord)) continue;

    return 0;
  }

  return 1;
}

int (KBCRead)(uint8_t port, uint32_t *output){

  uint32_t status;
  int tries = MAX_ATTEMPS;

  while(tries){

    if(sys_inb(KBC_IN_CMD,&status)) return 1;

    if( (status & ENABLE_INT) ){

      if( status & (TIMEOUT_ERROR | PARITY_ERROR) ) return 1;

      if(sys_inb(port, output)) return 1;

      if( !(status & MOUSE_TICK) ) return 1;

      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    tries--;
  }

  return 1;
}

int (mouse_subscribe)(uint8_t *bit_no){
  if(bit_no == NULL) return 1;
  (*bit_no) = BIT(hook_id_mouse);
  if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)) return 1;
  printf("mouse subscribe successful\n");
  return 0;
}

int (mouse_unsubscribe)(){
  if(sys_irqrmpolicy(&hook_id_mouse)) return 1;
  return 0;
}

void (mouse_ih)(){
  if(KBCRead(KBC_WRITE_CMD, &byte)) printf("ERROR");
}

int (mouseSync)(){

  if(mouse_counter == 0 && ( byte & MOUSE_SYNC) ){
    mouseBytes[0]= byte;
    mouse_counter++;
  }
  else{
    mouseBytes[mouse_counter] = byte;
    mouse_counter++;
  }

  return 0;
}

int (mousePacket)(){
  for (int i = 0 ; i < 3 ; i++) {
    mouseP.bytes[i] = mouseBytes[i];
  }

  mouseP.lb = mouseBytes[0] & MOUSE_LB;
  mouseP.mb = mouseBytes[0] & MOUSE_MB;
  mouseP.rb = mouseBytes[0] & MOUSE_RB;
  mouseP.x_ov = mouseBytes[0] & MOUSE_X_SIGNAL;
  mouseP.y_ov = mouseBytes[0] & MOUSE_Y_SIGNAL;
  mouseP.delta_x = (mouseBytes[0] & MOUSE_X_DELTA) ? (0xff00 | mouseBytes[1]) : mouseBytes[1];
  mouseP.delta_y = (mouseBytes[0] & MOUSE_Y_DELTA) ? (0xff00 | mouseBytes[2]) : mouseBytes[2];

  return 0;
}

void updateMouseLocation(){
  if(x+mouseP.delta_x < 0 && !mouseP.x_ov){
    x = 0;
  }
  else if(x+mouseP.delta_x > 800 && !mouseP.x_ov){
    x = 800;
  }
  else if(!mouseP.x_ov){
    x+=mouseP.delta_x;
  }

  if(y-mouseP.delta_y < 0 && !mouseP.y_ov){
    y = 0;
  }
  else if(y-mouseP.delta_y > 600 && !mouseP.y_ov){
    y = 600;
  }
  else if(!mouseP.y_ov){
    y-=mouseP.delta_y;
  }
}

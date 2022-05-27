
/* Variáveis úteis e globais */

#define BIT(n) (1 << (n))
int hook_id_mouse = 4;  // pode ser um número qualquer, desde que não seja igual a outro dispositivo a ser usado
uint8_t mouseBytes[3];  // vai conter os raw bytes
int counter = 0;        // o counter pertence a {0..3} e é inicializado a zero
uint32_t byte;          // o byte a ser lido na iteração corrente
struct packet mouseP;   // declarada no lab4 como extern packet mouseP

int (mouse_subscribe)(uint8_t *bit_no){
  if(bit_no == NULL) return 1;
  *bit_no = BIT(hook_id_mouse);                     // ativar o bit correspondente
  if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)) return 1;
  return 0;
}

int (mouse_unsubscribe)(){
  if(sys_irqrmpolicy(&hook_id_mouse)) return 1;
  return 0;
}

int (mouse_config)(uint8_t controlWord){

  uint32_t mouseResponse;

  do{
    // dizer à data port (0x60) que queremos escrever
    if(KBCWrite(0x64,0xD4)) return 1;

    // escrever a control word na porta Write (0x64)
    if(KBCWrite(0x60,controlWord)) return 1;
    tickdelay(micros_to_ticks(20000));
    // verificar config foi bem sucedida
    if(sys_inb(0x60, &mouseResponse)) return 1;

    //if(mouseResponse != 0xfa) return 1;
  }while(mouseResponse != 0xfa);

  return 0;
}

int (KBCWrite)(uint8_t port, uint8_t controlWord){

  uint32_t status;
  int tries = 20;

  while(tries){
    tries--;
    if(sys_inb(0x64, &status)) continue;

    while((status & BIT(1))) {};
    if(sys_outb(port, controlWord)) continue; //escerver para o status

    if( status & ( BIT(6) | BIT(7))) return 1;
    return 0;
  }

  return 1;
}

int (KBCRead)(uint8_t port, uint32_t *output){

  uint32_t status;
  int tries = 20;

  while(tries){

    if(sys_inb(0x64,&status)) return 1;

    if( (status & BIT(0)) ){

      if( status & (BIT(6) | BIT(7)) ) return 1;

      if(sys_inb(port, output)) return 1;

      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    tries--;
  }

  return 1;
}

void (mouse_ih)(){
  if(KBCRead(0x60, &byte)) printf("ERROR");
}

int (mouseSync)(){

  if(counter == 0 && ( byte & BIT(3)) ){
    mouseBytes[0]= byte;
    counter++;
  }
  else{
    mouseBytes[counter] = byte;
    counter++;
  }

  return 0;
}

int (mousePacket)(){
  for (int i = 0 ; i < 3 ; i++) {
    mouseP.bytes[i] = mouseBytes[i];
  }

  mouseP.lb = mouseBytes[0] & BIT(0);
  mouseP.mb = mouseBytes[0] & BIT(2);
  mouseP.rb = mouseBytes[0] & BIT(1);
  mouseP.x_ov = mouseBytes[0] & BIT(6);
  mouseP.y_ov = mouseBytes[0] & BIT(7);
  mouseP.delta_x = (mouseBytes[0] & BIT(4)) ? (0xff00 | mouseBytes[1]) : mouseBytes[1];
  mouseP.delta_y = (mouseBytes[0] & BIT(5)) ? (0xff00 | mouseBytes[2]) : mouseBytes[2];

  return 0;
}

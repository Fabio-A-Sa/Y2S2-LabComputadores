
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
  if(sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse)) return 1; // modificar a política e ativar as interrupções
  return 0;
}

int (mouse_unsubscribe)(){
  if(sys_irqrmpolicy(&hook_id_mouse)) return 1;     // des-subscrever as interrupções para aquele hook_id específico
  return 0;
}

int (mouse_config)(uint8_t controlWord){

  uint32_t mouseResponse;                           // a resposta do Mouse para a escrita pode ser ACK, NACK e ERROR
                                                    // se for ACK (0xFA) tudo OK, se for NACK (0xFE), tentar denovo
  do{
    if(KBCWrite(0x64,0xD4)) return 1;               // dizer a 0x64 que queremos escrever
    if(KBCWrite(0x60,controlWord)) return 1;        // escrever a control word para 0x60
    sleep(20000);                                   // esperar pela resposta do rato
    if(sys_inb(0x60, &mouseResponse)) return 1;     // ler imediatamente de 0x60 a mensagem de retorno do rato
  }while(mouseResponse != 0xfa);                    // até que o mouse receba o pedido e diga tudo OK (ACK)

  return 0;
}

int (KBCWrite)(uint8_t port, uint8_t controlWord){

  uint32_t status;
  int tries = 20;

  while(tries){                                     // tentar algumas vezes e sleep(20000) entre as tentativas
    tries--;
    if(sys_inb(0x64, &status)) continue;            // Ler o status do registo 0x64
    while(status & BIT(1)) {};                      // Esperar até que o status tenha o bit para o mouse
    if( status & ( BIT(6) | BIT(7))) return 1;      // Se não houver erros de paridade nem de timeout   
    if(sys_outb(port, controlWord)) continue;       // Escerver para a porta a controlWord desejada
    return 0;
  }

  return 1;
}

int (KBCRead)(uint8_t port, uint32_t *output){

  uint32_t status;
  int tries = 20;

  while(tries){                                     // tentar algumas vezes e sleep(20000) entre as tentativas
    if(sys_inb(0x64,&status)) return 1;             // lê o status
    if( (status & BIT(0)) ){                        // se a informação for do rato
      if( status & (BIT(6) | BIT(7)) ) return 1;    // e se não houver erros de timehout nem de paridade
      if(sys_inb(port, output)) return 1;           // então coloca no output o valor lido da porta pretendida
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    tries--;
  }

  return 1;
}

void (mouse_ih)(){
  if(KBCRead(0x60, &byte)) printf("ERROR");         // a cada iteração lê um byte da porta 60
}

int (mouseSync)(){

  if(counter == 0 && ( byte & BIT(3)) ){            // se for o primeiro byte do pacote, sincronizar
    mouseBytes[0]= byte;
    counter++;
  }
  else{
    mouseBytes[counter] = byte;
    counter++;
  }

  return 0;
}

int (mousePacket)(){                                // colocar a informação na struct facultada pelos professores
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

int (mouse_test_packet)(uint32_t cnt) {

    if(mouse_config(0xEA)) return 1;                // configura para o modo de stream
    if(mouse_config(0xF4)) return 1;                // Faz enable do mouse reporting
    if (mouse_subscribe(&irq_set) != 0) return 1;   // Subscreve as interrupções do mouse

    while(cnt) {

      if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
          printf("Error");
          continue;
      }

      if(is_ipc_notify(ipc_status)) {
          switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                  if (msg.m_notify.interrupts & irq_set) {

                    
                      mouse_ih();                           // lê mais um byte da porta 0x60
                      mouseSync();                          // sincroniza a raw data e aumenta o contador interno
                      if(counter == 3){                     // se o counter está a 3, então o packet está completo
                        mousePacket();                      // trata da raw data
                        mouse_print_packet(&mouseP);        // printa a informação
                        counter = 0;                        // começa a contar os bytes denovo  
                        cnt--;                              // o número de pacotes restantes a ler diminui
                      }

                  }
            }
        }
    }

    if (mouse_unsubscribe()) return 1;              // Deixa de subscrever as interrupções do mouse
    if(mouse_config(0xF5)) return 1;                // Faz disable ao data reporting

    return 0;
}
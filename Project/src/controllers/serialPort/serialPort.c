#include "serialPort.h"
#include "../video/graphics.h"
#include "../../game/macros.h"

int sp_hook_id = 7;
static Queue *inQueue;

extern int drawingSize;
extern int drawingColor;
extern uint8_t drawPackets[11];
extern int countDrawPackets;
extern char word[24];
extern int gameState;
extern bool guessStartedHandShake;
extern int wordSize;
extern int wordBytes;

int sp_subscribe_int(uint8_t* bitno){
    *bitno = BIT(sp_hook_id);
    if(sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &sp_hook_id)) return 1;
    return 0;
}

int sp_unsubscribe_int(){
    if(sys_irqrmpolicy(&sp_hook_id)) return 1;
    return 0;
}

int sp_get_status(uint8_t* status){
    return util_sys_inb(COM1_UART_BASE + LSR, status);
}

void sp_init(){
    uint8_t ier;
    if(util_sys_inb(COM1_UART_BASE+ IER, &ier)) return;
    ier &= 0xF0;
    if(sys_outb(COM1_UART_BASE + IER,ier | IER_ERBFI)) return;
    inQueue = createQueue();
}

void sp_exit(){
    clear(inQueue);
}

int send_byte(uint8_t byte){
    uint8_t status, attempts = 10;
    while(attempts--){
      if(sp_get_status(&status)) return 1;
      if(status & LSR_THRE)
        return sys_outb(COM1_UART_BASE + THR,byte);
    }
    return 1;
}

int read_byte(){
    uint8_t status, rbr;
    if(sp_get_status(&status)) return 1;
    if(status & LSR_DATA_READY){
      if(util_sys_inb(COM1_UART_BASE + RBR, &rbr)) return 1;
      if(!(status & (LSR_OE| LSR_PE| LSR_FE))){
          push(inQueue,rbr);
          return 0;
      }
    }
    return 1;
}

Queue* get_inQueue(){
    return inQueue;
}

int SP_clearInterrupts(){
    if(sys_outb(COM1_UART_BASE + FCR,FCR_CLEAR)) return 1;
    while(!isEmpty(inQueue))
        pop(inQueue);
    return 0;
}

void sp_ih(){
    uint8_t iir;
    if(util_sys_inb(COM1_UART_BASE+ IIR, &iir)) return;
    if(!(iir & IIR_NO_INT_PENDING))
        if(iir & IIR_ID == IIR_FIFO_CT)
            while(read_byte());
}

int processWaitingDrawSP(uint8_t val){
    if(val == 0xFE){
        send_byte(0xFD);
        tickdelay(5);
        uint8_t len = strlen(word) & 0xFF;
        send_byte(len);
        for(unsigned i = 0; i <= len; i++){
            tickdelay(1);
            send_byte(word[i]); 
        }
        gameState = 2;
    } 
    return 0;
}

int processWaitingGuessSP(uint8_t val){
    if(val == 0xFF){
        send_byte(0xFE);
        guessStartedHandShake = true;
    }
    else if(guessStartedHandShake && val == 0xFD){
        gameState = 1;
        guessStartedHandShake = false;
    }
    return 0;
}

int processLoadingGuessSP(uint8_t val){
    if(val != 0){
        if(wordSize == 0){
            wordSize = val;
        }
        else if (wordBytes < wordSize){
            word[wordBytes++] = val;
        }
    }
    else if(wordBytes == wordSize) {
        gameState = 2;
        countDrawPackets = 0;
    }
    return 0;
}

int processPlayingDrawSP(uint8_t val){
    if(val == 0x42){
        printf("game end\n");
        gameState = 3;
    }
    return 0;
}

int processPlayingGuessSP(uint8_t val){
    while(countDrawPackets < 11){
        drawPackets[countDrawPackets++] = val;
        sp_ih();
        val = pop(get_inQueue());
    }
    if(drawPackets[0] == 0xEA && drawPackets[1] == 0xEA && drawPackets[2] == 0xEA) gameState = 3;
    else if(countDrawPackets == 11){
        countDrawPackets = 0; 
        if(drawPackets[0] == CONTROL1COLOR && drawPackets[1] == CONTROL2COLOR && drawPackets[2] == CONTROL3COLOR){
            drawingSize = drawPackets[4];
            switch(drawPackets[3]){
            case 0: drawingColor = COLOR_BLACK; break;
            case 1: drawingColor = COLOR_GRAY; break;
            case 2: drawingColor = COLOR_RED; break;
            case 3: drawingColor = COLOR_YELLOW; break;
            case 4: drawingColor = COLOR_GREEN; break;
            case 5: drawingColor = COLOR_AQUA; break;
            case 6: drawingColor = COLOR_DARKBLUE; break;
            case 7: drawingColor = COLOR_PURPLE; break;
            }
        }
        else if( (drawPackets[0] == DRAWLINE && drawPackets[1] == DRAWLINE) || (drawPackets[0] == ERASELINE && drawPackets[1] == ERASELINE) ) {
            uint16_t received_prevX = (drawPackets[3] << 8) + drawPackets[4];
            uint16_t received_prevY = (drawPackets[5] << 8) + drawPackets[6];
            uint16_t received_X = (drawPackets[7] << 8) + drawPackets[8];
            uint16_t received_Y = (drawPackets[9] << 8) + drawPackets[10];
            if( (DRAW_LOWERBD_Y > received_prevY) || (received_prevY > DRAW_UPPERBD_Y) || (DRAW_LOWERBD_Y > received_Y) || (received_Y > DRAW_UPPERBD_Y)) {SP_clearInterrupts();  return 1;}
            else if( (DRAW_LOWERBD_X > received_prevX) || (received_prevX > DRAW_UPPERBD_X) || (DRAW_LOWERBD_X > received_X) || (received_X > DRAW_UPPERBD_X)) {SP_clearInterrupts(); return 1;}
            drawToCanvas( received_prevX, //prevX
                          received_prevY, //prevY
                          received_X, //x
                          received_Y, //y
                          (drawPackets[0] == ERASELINE) ? COLOR_WHITE : drawingColor); 
        }
    }
    SP_clearInterrupts();
    return 0;
}

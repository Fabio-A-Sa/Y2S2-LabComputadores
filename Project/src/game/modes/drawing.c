#include "drawing.h"
#include "../game.h"
#include "../../controllers/serialPort/serialPort.h"
#include "../macros.h"

uint32_t drawingColor = COLOR_BLACK;
uint8_t drawingColorCode = 0;
int drawingSize = 12;
int input_counter = 0;
int prevX, prevY;
extern int gameMode;
extern int gameState;

extern char word[24];

int processGamingLeftClick(int x, int y){
    if(gameState == 2){
        if( y>DRAW_LOWERBD_Y && y <DRAW_UPPERBD_Y){
            send_byte( DRAWLINE);
            send_byte( DRAWLINE);
            send_byte( DRAWLINE);
            send_byte( prevX >> 8 );
            send_byte( prevX );
            send_byte( prevY >> 8 );
            send_byte( prevY );
            send_byte( x >> 8 );
            send_byte( x );
            send_byte( y >> 8 );
            send_byte( y );
            if(drawToCanvas(prevX,prevY,x,y,drawingColor)) return 1;
        }
        else{
            if(y > 10 && y < 60){
                if(x > 15 && x < 65) {drawingColor = COLOR_BLACK; drawingColorCode = 0;} // black color button
                else if(x > 94 && x < 144) {drawingColor = COLOR_GRAY; drawingColorCode = 1;} //Gray color button
                else if(x > 173 && x < 223) {drawingColor = COLOR_RED; drawingColorCode = 2;} //Red color button
                else if(x > 252 && x < 302) {drawingColor = COLOR_YELLOW; drawingColorCode = 3;} //Yellow color button
                else if(x > 331 && x < 381) {drawingColor = COLOR_GREEN; drawingColorCode = 4;} //Green color button
                else if(x > 410 && x < 460) {drawingColor = COLOR_AQUA; drawingColorCode = 5;} //Aqua color button
                else if(x > 489 && x < 539) {drawingColor = COLOR_DARKBLUE; drawingColorCode = 6;} //Dark blue color button
                else if(x > 568 && x < 618) {drawingColor = COLOR_PURPLE; drawingColorCode = 7;} //Purple color button
                else if(x > 647 && x < 697) {drawingSize = (drawingSize >= 6) ? drawingSize-2 : 4;} //Descrease brush size button
                else if(x > 726 && x < 776) {drawingSize = (drawingSize <= 20) ? drawingSize+2 : 22;} //Increse brush size button

                send_byte (CONTROL1COLOR);
                send_byte (CONTROL2COLOR);
                send_byte (CONTROL3COLOR);
                send_byte (drawingColorCode);
                send_byte (drawingSize);
                for(int i=0; i< 6; ++i) send_byte(0x00);
            }
        }
    }
    return 0;
}

int processGamingRightClick(int x, int y){
    if(gameState == 2)
        send_byte( ERASELINE);
        send_byte( ERASELINE);
        send_byte( ERASELINE);
        send_byte( prevX >> 8 );
        send_byte( prevX );
        send_byte( prevY >> 8 );
        send_byte( prevY );
        send_byte( x >> 8 );
        send_byte( x );
        send_byte( y >> 8 );
        send_byte( y );
        if(drawToCanvas(prevX,prevY,x,y,COLOR_WHITE)) return 1;
    return 0;
}

int processWord(){
    if(readWord()) return 1;
    if(wordInterpreter(word, 545)) return 1;
    return 0;
}

void KB_DrawingControls(uint8_t scancode){
     switch(scancode){
        case MAKE_ESC:
            if(gameState == 2) {send_byte(0xEA);send_byte(0xEA);send_byte(0xEA);}
            gameMode = MENU_MODE;
            gameState = 0;
            SP_clearInterrupts();
            memset(word, '\0', sizeof(word));
            break;
        case MAKE_ENTER:
            break;
        case MAKE_1:
            drawingColor = COLOR_BLACK;
            drawingColorCode = 0;
            break;
        case MAKE_2:
            drawingColor = COLOR_GRAY;
            drawingColorCode = 1;
            break; 
        case MAKE_3:
            drawingColor = COLOR_RED;
            drawingColorCode = 2;
            break; 
        case MAKE_4:
            drawingColor = COLOR_YELLOW;
            drawingColorCode = 3;
            break; 
        case MAKE_5:
            drawingColor = COLOR_GREEN;
            drawingColorCode = 4;
            break; 
        case MAKE_6:
            drawingColor = COLOR_AQUA;
            drawingColorCode = 5;
            break; 
        case MAKE_7:
            drawingColor = COLOR_DARKBLUE;
            drawingColorCode = 6;
            break; 
        case MAKE_8:
            drawingColor = COLOR_PURPLE;
            drawingColorCode = 7;
            break;
        case MAKE_MINUS:
            drawingSize = (drawingSize >= 6) ? drawingSize-2 : 4;
            break;
        case MAKE_PLUS:
            drawingSize = (drawingSize <= 20) ? drawingSize+2 : 22;
            break;
        default:
            break;
    }
    if(gameState == 2){
        send_byte (CONTROL1COLOR);
        send_byte (CONTROL2COLOR);
        send_byte (CONTROL3COLOR);
        send_byte (drawingColorCode);
        send_byte (drawingSize);
        send_byte (0x00);
        send_byte (0x00);
        send_byte (0x00);
        send_byte (0x00);
    }
}

int drawDrawingGame(){

    copyDrawingToBuffer();
    print_xpm((xpm_map_t) one_xpm, 32, 70);
    print_xpm((xpm_map_t) two_xpm, 111, 70);
    print_xpm((xpm_map_t) three_xpm, 190, 70);
    print_xpm((xpm_map_t) four_xpm, 269, 70);
    print_xpm((xpm_map_t) five_xpm, 348, 70);
    print_xpm((xpm_map_t) six_xpm, 427, 70);
    print_xpm((xpm_map_t) seven_xpm, 506, 70);
    print_xpm((xpm_map_t) eight_xpm, 585, 70);
    print_xpm((xpm_map_t) minus_xpm, 664, 70);
    print_xpm((xpm_map_t) plus_xpm, 743, 70);
    wordInterpreter(word, 545);
    gamePlayCounterDraw();
    if(gameState == 3){
        wordInterpreter("end", 300);
    }
    else if(gameState == 1 || (gameMode == DRAW_MODE && gameState == 0)){
        wordInterpreter("loading", 300);
    }
  return 0;
}

#include "guessing.h"
#include "../game.h"
#include "../../controllers/serialPort/serialPort.h"

extern int gameMode;

void KB_InstControls(uint8_t scancode){
    switch(scancode){
        case MAKE_ESC:
            gameMode = MENU_MODE;
        default:
            break;
    }    
}

int drawInstPage(){
    copyDrawingToBuffer();
    return 0;
}

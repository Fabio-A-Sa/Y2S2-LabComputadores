#include "menu.h"
#include "lcom/vbe.h"
#include "../game.h"
#include "../../controllers/serialPort/serialPort.h"

//extern
extern struct packet mouseP;
//extern
extern bool gameRunning;
extern int gameMode;

//extern game
extern int gameplayCounter;

//display externs
extern uint8_t *imgBuffer;
extern uint8_t *drawBuffer;
extern uint16_t xRes, yRes;
extern unsigned bytesPerPixel;

bool startDrawPressed = false;
bool startGuessPressed = false;
bool quitPressed = false;
bool instrPressed = false;



int processStartDrawButton(uint16_t x, uint16_t y){
    if( x> 150 && x<350 && y>300 && y < 400){
        if(!startDrawPressed)
            if(mouseP.lb) 
                startDrawPressed = true;
        if(startDrawPressed)
            if(!mouseP.lb){
                gameMode = DRAW_MODE;
                gameplayCounter = 120;
                clearDrawBuffer();
                headerCompose();
                footerCompose();
                processWord();
                startDrawPressed = false;
            }
    }
    else startDrawPressed = false;
    return 0;
}

int processStartGuessButton(uint16_t x, uint16_t y){
    if( x>450 && x < 650 && y>300 && y < 400){
        if(!startGuessPressed)
            if(mouseP.lb) 
                startGuessPressed = true;
        if(startGuessPressed)
            if(!mouseP.lb){
                gameMode = GUESS_MODE;
                gameplayCounter = 120;
                clearDrawBuffer();
                memset(drawBuffer,0xDDDDDD,xRes*100*bytesPerPixel);
                footerCompose();
                startGuessPressed = false;
            }
    }
    else startGuessPressed = false;
    return 0;
}

int processInstrButton(uint16_t x, uint16_t y){
    if( x> 150 && x<350 && y>420 && y < 520){
        if(!instrPressed)
            if(mouseP.lb) 
                instrPressed = true;
        if(instrPressed)
            if(!mouseP.lb){
                gameMode = INSTR_MODE;
                clearDrawBuffer();
                drawXpmDrawBuffer( (xpm_map_t) instPage_xpm, 0, 0);
                instrPressed = false;
            }
        }
    else instrPressed = false;
    return 0;
}

int processQuitButton(uint16_t x, uint16_t y){
    if( x>450 && x < 650 && y>420 && y < 520){
        if(!quitPressed)
            if(mouseP.lb)
                quitPressed = true;
        if(quitPressed)
            if(!mouseP.lb){
                gameRunning = false;
                quitPressed = false;
            }
    }
    else quitPressed = false;
    return 0;
}

void KB_MenuControls(uint8_t scancode){
    switch(scancode){
        case MAKE_ESC:
            gameRunning = false;
            break;
        case 0x02:
            gameMode = DRAW_MODE;
            gameplayCounter = 120;
            clearDrawBuffer();
            headerCompose();
            footerCompose();
            processWord();
            break;
        case 0x03:
            gameMode = GUESS_MODE;
            gameplayCounter = 120;
            clearDrawBuffer();
            memset(drawBuffer,0xDDDDDD,xRes*100*bytesPerPixel);
            footerCompose();
            break;
        case 0x04:
            gameMode = INSTR_MODE;
            clearDrawBuffer();
            drawXpmDrawBuffer( (xpm_map_t) instPage_xpm, 0, 0);
            break;
        default:
            break;
    }
}

int drawMenu(){

  memset(imgBuffer, 0xDDDDDD, xRes*yRes*bytesPerPixel);
  //draw title
  drawSprite(gameTitle,214,100);
  //draw button
  if(startDrawPressed) print_xpm((xpm_map_t) drawButtonPressed_xpm,150,300);
  else drawSprite(drawButton,150,300);
  //guess button
  if(startGuessPressed) print_xpm((xpm_map_t) guessButtonPressed_xpm,450,300);
  else drawSprite(guessButton,450,300);
  //instruction button
  if(instrPressed) print_xpm((xpm_map_t) instButtonPressed_xpm,150,420);
  else drawSprite(instButton,150,420);
  //quit button
  if(quitPressed) print_xpm((xpm_map_t) quitButtonPressed_xpm,450,420);
  else drawSprite(quitButton,450,420);

  return 0;
}

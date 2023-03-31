/*! \mainpage Gertic-- : A simple recreating of the game Gartic but as a small operating system using minix as a kernel
 *
 * This project uses the knowledge aquired in LCOM classes of how to build driver for different components of a computer to build a simple
 * game. It uses concepts of how to use a computer timer, keyboard, mouse, display (gpu), and others.
 *
 * This work was done within the scope of the Compute Laboratory course,
 * and aimed at deepening the knowledge about to use the inteface of common peripherals, and program in C for low level applications.
 *
 * All these recreates a simple OS where a player is able to draw and another is able to guess what it is being drawn.
 * It uses the mouse to draw to the screen and select buttons, the keyboard to type and use button shortcuts, graphics (gpu) to display the
 * GUI of the game, the timer to control flow o information and play time, serial port to control the sharing of information between the different machines, 
 * and many more.
 *
 * This project was made by:
 * @author Fábio Araújo de Sá, Inês Sá Pereira Estêvão Gaspar, Lourenço Alexandre Correia Gonçalves, Marcos William Ferreira Pinto
 *
 * @date 11/06/2022
 */

#include <lcom/lcf.h>
#include <string.h>
#include "game/game.h"
#include "controllers/video/graphics.h"
#include "sprites/sprites.h"
#include "game/modes/drawing.h"
#include "game/modes/guessing.h"
#include "game/modes/inst.h"
#include "game/modes/menu.h"
#include "controllers/video/VBE.h"
#include "controllers/keyboardMouse/keyboard.h"
#include "controllers/keyboardMouse/i8042.h"
#include "controllers/keyboardMouse/mouse.h"
#include "controllers/serialPort/serialPort.h"
#include "controllers/serialPort/queue.h"
#include "game/UTILS.h"

bool gameRunning = true;
int gameMode = MENU_MODE;
bool currentGaming = false;
//keyboard externs
extern uint8_t scancode;
//display externs
extern vbe_mode_info_t info;
extern uint8_t *imgBuffer;
extern uint16_t xRes;
extern uint16_t yRes;
extern unsigned bytesPerPixel;
//mouse externs
extern int x;
extern int y;
extern int mouse_counter;
extern struct packet mouseP;
extern uint8_t mouseBytes[3];
//timer externs
extern int counter_timer;
//game extern
extern int gameplayCounter;
extern int prevX, prevY;

//extern drawing
extern char word[24];
int wordBytes = 0;
uint8_t wordSize = 0;
extern int drawingSize;
extern uint32_t drawingColor;

typedef enum { WAITING, LOADING, PLAYING, ENDING} GameState;
GameState gameState = WAITING;
bool guessStartedHandShake = false;
int drawBytes = 0;

uint8_t drawPackets[11];
int countDrawPackets = 0;

int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}

int process_interrupts(){

  if(mouse_config(0xEA)) return 1; // enable stream mode
  if(mouse_config(0xF4)) return 1; // enable data report
  sp_init(); //initiate serial Port

  if(timer_set_frequency(0,30)) return 1;

  int ipc_status;
  uint8_t irq_kb, irq_mouse, irq_timer, irq_set_sp;
  message msg;

  irq_kb = 2;

  if(timer_subscribe_int(&irq_timer)) return 1;
  if(subscribe_KBC_interrupts(&irq_kb)) return 1;
  if(mouse_subscribe(&irq_mouse)) return 1;
  //if(sp_subscribe_int(&irq_sp)) return 1;
  if (sp_subscribe_int(&irq_set_sp) != 0){
    printf("Error subscribing serial port\n");
    return 1;
  }

  while(gameRunning){

    if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
        printf("Error");
        continue;
    }

    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          //timer
          if (msg.m_notify.interrupts & irq_timer) {
            counter_timer++;
            if(gameMode == MENU_MODE) {drawMenu();}
            else if( gameMode == DRAW_MODE) {drawDrawingGame();}
            else if( gameMode == GUESS_MODE) {drawGuessingGame();}
            else if ( gameMode == INSTR_MODE) {drawInstPage();}

            //mouse prints
            printMouse();
            //send img to display
            copyBufferToVideo();
            //clear buffer to start next frame
            clearImgBuffer();

            if(counter_timer % 30 == 0){
              if(gameState == PLAYING) gameplayCounter--;
              if( (gameMode == DRAW_MODE) && (gameState == WAITING) ) send_byte(0xFF);
              if( (gameplayCounter == 0) && (gameState == PLAYING) ){
                gameState = ENDING;
              }
            }

          }
          //Keyboard
          if (msg.m_notify.interrupts & irq_kb) {
            kbc_ih();
            if( gameMode == MENU_MODE) { KB_MenuControls(scancode); }
            else if ( gameMode == DRAW_MODE) { KB_DrawingControls(scancode); }
            else if ( gameMode == GUESS_MODE) { KB_GuessingControls(scancode); }
            else if ( gameMode == INSTR_MODE) { KB_InstControls(scancode); }
          }
          //mouse
          if (msg.m_notify.interrupts & irq_mouse) {
            mouse_ih();
            mouseSync();
            if(mouse_counter == 3){
              mouse_counter = 0;
              mousePacket();
              updateMouseLocation();
              if ( gameMode == MENU_MODE) {
                processStartDrawButton(x, y);
                processStartGuessButton(x, y);
                processQuitButton(x, y);
                processInstrButton(x, y);
              }
              else if( gameMode == DRAW_MODE) {
                if(mouseP.lb){
                  processGamingLeftClick(x, y);
                }
                else if(mouseP.rb){
                  processGamingRightClick(x, y);
                }
              }
              prevX = x; prevY = y;
            }
          }
          //Serial port
          if (msg.m_notify.interrupts & irq_set_sp){

            sp_ih();
            uint8_t val = pop(get_inQueue());

            if(gameMode == DRAW_MODE){
              switch(gameState){
                case WAITING:
                  processWaitingDrawSP(val);
                  break;
                case PLAYING:
                  processPlayingDrawSP(val);
                  break;
                default:
                  break;
              }
            }
            else if(gameMode == GUESS_MODE){
              switch(gameState){
                case WAITING:
                   processWaitingGuessSP(val);
                  break;
                case LOADING:
                  processLoadingGuessSP(val);
                  break;
                case PLAYING:
                  processPlayingGuessSP(val);
                  break;
                default:
                  break;
              }
            }
            else{
              SP_clearInterrupts();
            }

          }
          break;
        default:
          break;
      }
    }
  }
  if(mouse_unsubscribe()) return 1;
  if(unsubscribe_KBC_interrupts()) return 1;
  if(mouse_config(0xF5)) return 1;
  if(sp_unsubscribe_int()) return 1;
  return 0;
}


int (proj_main_loop)(int argc, char *argv[]) {

  if(map_vmem(VBE_600p_DC)) return 1;
  if(init_graphics_mode(VBE_600p_DC)) return 1;
  loadSprites();
  allocateImgBuffer();
  allocateDrawBuffer();
  if (process_interrupts()) return 1;
  if (vg_exit()) return 1;
  unloadSprites();
  freeImgBuffer();
  freeDrawBuffer();
  return 0;
}

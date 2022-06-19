#include "guessing.h"
#include "../game.h"
#include "../../controllers/serialPort/serialPort.h"

extern int gameMode;
extern int gameState;
extern int wordBytes;
extern int wordSize;
extern char word[24];
char inputWord[24];
int inputWordCounter = 0;

void KB_GuessingControls(uint8_t scancode){
    if(inputWordCounter < 24){
        switch(scancode){
            case MAKE_ESC:
                if(gameState == 2) send_byte(0x42);
                gameMode = MENU_MODE;
                gameState = 0;
                memset(inputWord, '\0', sizeof(inputWord));
                memset(word, '\0', sizeof(word));
                wordBytes = 0;
                wordSize = 0;
                SP_clearInterrupts();
                break;
            case MAKE_ENTER:
                if(gameState != 2) return;
                if(!compareGuess()){
                    gameState = 3;
                    send_byte(0x42);
                }
                memset(inputWord, '\0', sizeof(inputWord));
                inputWordCounter = 0;
                break;
            case MAKE_A: inputWord[inputWordCounter++] = 'a'; break;
            case MAKE_B: inputWord[inputWordCounter++] = 'b'; break;
            case MAKE_C: inputWord[inputWordCounter++] = 'c'; break;
            case MAKE_D: inputWord[inputWordCounter++] = 'd'; break;
            case MAKE_E: inputWord[inputWordCounter++] = 'e'; break;
            case MAKE_F: inputWord[inputWordCounter++] = 'f'; break;
            case MAKE_G: inputWord[inputWordCounter++] = 'g'; break;
            case MAKE_H: inputWord[inputWordCounter++] = 'h'; break;
            case MAKE_I: inputWord[inputWordCounter++] = 'i'; break;
            case MAKE_J: inputWord[inputWordCounter++] = 'j'; break;
            case MAKE_K: inputWord[inputWordCounter++] = 'k'; break;
            case MAKE_L: inputWord[inputWordCounter++] = 'l'; break;
            case MAKE_M: inputWord[inputWordCounter++] = 'm'; break;
            case MAKE_N: inputWord[inputWordCounter++] = 'n'; break;
            case MAKE_O: inputWord[inputWordCounter++] = 'o'; break;
            case MAKE_P: inputWord[inputWordCounter++] = 'p'; break;
            case MAKE_Q: inputWord[inputWordCounter++] = 'q'; break;
            case MAKE_R: inputWord[inputWordCounter++] = 'r'; break;
            case MAKE_S: inputWord[inputWordCounter++] = 's'; break;
            case MAKE_T: inputWord[inputWordCounter++] = 't'; break;
            case MAKE_U: inputWord[inputWordCounter++] = 'u'; break;
            case MAKE_V: inputWord[inputWordCounter++] = 'v'; break;
            case MAKE_W: inputWord[inputWordCounter++] = 'w'; break;
            case MAKE_X: inputWord[inputWordCounter++] = 'x'; break;
            case MAKE_Y: inputWord[inputWordCounter++] = 'y'; break;
            case MAKE_Z: inputWord[inputWordCounter++] = 'z'; break;
            default:
                break;
        }
        
    }
    if(scancode == MAKE_BACKSPACE){
        if(inputWordCounter > 0){
            inputWordCounter--;
            inputWord[inputWordCounter] = '\0';
        }
    }
}

int compareGuess(){
    printf("%s / %s\n", word, inputWord);
    return strcmp(word, inputWord);
}

int drawGuessingGame(){

    copyDrawingToBuffer();
    gamePlayCounterDraw();
    wordInterpreter(inputWord, 545);

    if(gameState == 3){
        wordInterpreter("end", 300);
    }
    else if(gameState == 1 || (gameMode == GUESS_MODE && gameState == 0)){
        wordInterpreter("loading", 300);
    }

    return 0;
}

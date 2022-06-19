#include "game.h"
#include "../sprites/sprites.h"

//game condition
extern int gameMode;
//mouse externs
extern int x;
extern int y;

int gameplayCounter = 120;

void (printMouse)() {
  if(gameMode == MENU_MODE){
    if( x> 150 && x<350){
        if( (y>300 && y < 400) || (y>420 && y <520)) drawSprite(handCursor,x,y);
        else  drawSprite(mouseCursor,x,y);
    }
    else if( x>450 && x < 650){
        if( (y>300 && y < 400) || (y>420 && y <520)) drawSprite(handCursor,x,y);
        else drawSprite(mouseCursor,x,y);
    }
    else drawSprite(mouseCursor,x,y);
  }
  else if (gameMode == DRAW_MODE){
    if( y > 100 & y < 500){
        drawSprite(brushCursor,x,y);
    }
    else{
        if( y>10 && y<60){
            int xTemp = x;
            while( xTemp > 75) xTemp-=79;
            if( xTemp>15 && xTemp<65) drawSprite(handCursor,x,y);
            else drawSprite(mouseCursor,x,y);
        }
        else drawSprite(mouseCursor,x,y);
    }
  }
}

//load word
int wordInterpreter(char* word, int yLevel){
    int xWord =25;
    for(int i=0; i<24; i++){
        if( *(word+i) == '\0') break;
        switch( *(word+i) ){
            case 'a': case 'A':
                print_xpm( (xpm_map_t) a_xpm,xWord,yLevel); break;
            case 'b': case 'B':
                print_xpm( (xpm_map_t) b_xpm,xWord,yLevel); break;
            case 'c': case 'C':
                print_xpm( (xpm_map_t) c_xpm,xWord,yLevel); break;
            case 'd': case 'D':
                print_xpm( (xpm_map_t) d_xpm,xWord,yLevel); break;
            case 'e': case 'E':
                print_xpm( (xpm_map_t) e_xpm,xWord,yLevel); break;
            case 'f': case 'F':
                print_xpm( (xpm_map_t) f_xpm,xWord,yLevel); break;
            case 'g': case 'G':
                print_xpm( (xpm_map_t) g_xpm,xWord,yLevel); break;
            case 'h': case 'H':
                print_xpm( (xpm_map_t) h_xpm,xWord,yLevel); break;
            case 'i': case 'I':
                print_xpm( (xpm_map_t) i_xpm,xWord,yLevel); break;
            case 'j': case 'J':
                print_xpm( (xpm_map_t) j_xpm,xWord,yLevel); break;
            case 'k': case 'K':
                print_xpm( (xpm_map_t) k_xpm,xWord,yLevel); break;
            case 'l': case 'L':
                print_xpm( (xpm_map_t) l_xpm,xWord,yLevel); break;
            case 'm': case 'M':
                print_xpm( (xpm_map_t) m_xpm,xWord,yLevel); break;
            case 'n': case 'N':
                print_xpm( (xpm_map_t) n_xpm,xWord,yLevel); break;
            case 'o': case 'O':
                print_xpm( (xpm_map_t) o_xpm,xWord,yLevel); break;
            case 'p': case 'P':
                print_xpm( (xpm_map_t) p_xpm,xWord,yLevel); break;
            case 'q': case 'Q':
                print_xpm( (xpm_map_t) q_xpm,xWord,yLevel); break;
            case 'r': case 'R':
                print_xpm( (xpm_map_t) r_xpm,xWord,yLevel); break;
            case 's': case 'S':
                print_xpm( (xpm_map_t) s_xpm,xWord,yLevel); break;
            case 't': case 'T':
                print_xpm( (xpm_map_t) t_xpm,xWord,yLevel); break;
            case 'u': case 'U':
                print_xpm( (xpm_map_t) u_xpm,xWord,yLevel); break;
            case 'v': case 'V':
                print_xpm( (xpm_map_t) v_xpm,xWord,yLevel); break;
            case 'w': case 'W':
                print_xpm( (xpm_map_t) w_xpm,xWord,yLevel); break;
            case 'x': case 'X':
                print_xpm( (xpm_map_t) x_xpm,xWord,yLevel); break;
            case 'y': case 'Y':
                print_xpm( (xpm_map_t) y_xpm,xWord,yLevel); break;
            case 'z': case 'Z':
                print_xpm( (xpm_map_t) z_xpm,xWord,yLevel); break;
            default:
                return 1;
        }
      xWord+=20;
    }

    return 0;
}

int gamePlayCounterDraw(){
  int yLevel = 545;
  int xWord =750;
  int tempGameCounter = gameplayCounter;
  int digit;
  while(tempGameCounter > 0){
    digit = tempGameCounter % 10;
    tempGameCounter /= 10;
    switch(digit){
      case 0:
          print_xpm( (xpm_map_t) zero_xpm,xWord,yLevel); break;
      case 1:
          print_xpm( (xpm_map_t) one_xpm,xWord,yLevel); break;
      case 2:
          print_xpm( (xpm_map_t) two_xpm,xWord,yLevel); break;
      case 3:
          print_xpm( (xpm_map_t) three_xpm,xWord,yLevel); break;
      case 4:
          print_xpm( (xpm_map_t) four_xpm,xWord,yLevel); break;
      case 5:
          print_xpm( (xpm_map_t) five_xpm,xWord,yLevel); break;
      case 6:
          print_xpm( (xpm_map_t) six_xpm,xWord,yLevel); break;
      case 7:
          print_xpm( (xpm_map_t) seven_xpm,xWord,yLevel); break;
      case 8:
          print_xpm( (xpm_map_t) eight_xpm,xWord,yLevel); break;
      case 9:
          print_xpm( (xpm_map_t) nine_xpm,xWord,yLevel); break;
      default:
        break;
    }
    
    xWord-=20;
  }
  
  return 0;
}

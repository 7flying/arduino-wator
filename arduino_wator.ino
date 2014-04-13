#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
// SE: Shark's (default) energy
#define SE 56
// RF: reproduce fish, chronons needed to reproduce
#define RF 100000
// RS: reproduce shark, chronons needed to reproduce 
#define RS 4
// Element structure
typedef struct {
 char type; // w: water, f: fish, s: shark
 uint8_t survivedChro; // number of survived chronons for fish or sharks
 uint8_t energy; // current energy, for sharks
} Element;

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
Element board [][8] = { { {'f',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'f',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'f',0,0} }
                      };
void setup() {
 matrix.begin(0x70);
 randomSeed(analogRead(0));
}
/* Paints the board  */
void paint() {
 matrix.clear();
 for(uint8_t i = 0; i < 8; i++) {
  for(uint8_t j = 0; j < 8; j++) {
    if(board[i][j].type == 's')
      matrix.drawPixel(i,j,LED_RED);
     else
       if(board[i][j].type == 'f')
         matrix.drawPixel(i,j, LED_YELLOW);
  } 
 }
 matrix.writeDisplay();
}
/* Increases the chronon */
void nextChronon() {
  for(uint8_t i = 0; i < 8; i++) {
    for(uint8_t j = 0; j < 8; j++) {
      if(board[i][j].type != 'w') {
        // Increase survived chronons
        board[i][j].survivedChro++;
        // Fish's behaviour
        if(board[i][j].type == 'f') {
          if(board[i][j].survivedChro % RF == 0)
            reproduce(i,j);
          else
            moveCreature(i,j);
        // Shark's behaviour
        } else {
          if(board[i][j].energy == 0) {
            // The shark dies
            board[i][j].type = 'w';
            board[i][j].survivedChro = 0;
          } else {
            board[i][j].energy--;
            if(board[i][j].survivedChro % RS == 0)
              reproduce(i, j);
            else
              moveCreature(i, j);
          }
        }   
      }
    }
  }
}
/* Reproduces the creature at (x, y) */
void reproduce(uint8_t x, uint8_t y) {
  board[x][y].survivedChro = 0;
  char type = board[x][y].type;
  if(moveCreature(x,y)) { // The creature succesfully moved
    board[x][y].type = type;
    if(type == 's')
      board[x][y].energy = SE;
  }
}
/* Moves randomly the creature at (i, j) */
boolean moveCreature(uint8_t i, uint8_t j) {
  return false; //TODO
}
void loop() {
 paint();
 delay(1000);
 nextChronon();
}

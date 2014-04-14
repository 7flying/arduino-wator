#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
// SE: Shark's (default) energy
#define SE 20
// RF: reproduce fish, chronons needed to reproduce
#define RF 5
// RS: reproduce shark, chronons needed to reproduce 
#define RS 15
// Element structure
typedef struct {
 char type; // w: water, f: fish, s: shark
 uint8_t survivedChro; // number of survived chronons for fish or sharks
 uint8_t energy; // current energy, for sharks
} Element;

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
Element board [][8] = { { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'f',0,0}, {'w',0,0}, {'w',0,0}, {'f',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'f',0,0}, {'s',0,SE}, {'f',0,0}, {'f',0,0} },
                        { {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'f',0,0} },
                        { {'w',0,0}, {'f',0,0}, {'s',0,SE}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'f',0,0} },
                        { {'w',0,0}, {'f',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} },
                        { {'w',0,0}, {'f',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'s',0,SE}, {'w',0,0}, {'w',0,0} },
                        { {'f',0,0}, {'f',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0}, {'w',0,0} }
                      };
void setup() {
 matrix.begin(0x70);
 randomSeed(analogRead(0));
 //Serial.begin(9600);
}
/* Paints the board  */
void paint() {
 matrix.clear();
 for(uint8_t i = 0; i < 8; i++) {
  for(uint8_t j = 0; j < 8; j++) {
    if(board[i][j].type == 's')
      matrix.drawPixel(j,i,LED_RED);
     else
       if(board[i][j].type == 'f')
         matrix.drawPixel(j,i, LED_YELLOW);
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
void reproduce(uint8_t i, uint8_t j) {
  board[i][j].survivedChro = 0;
  char type = board[i][j].type;
  if(moveCreature(i,j)) { // The creature succesfully moved
    board[i][j].type = type;
    if(type == 's')
      board[i][j].energy = SE;
  }
}
/* Moves randomly the creature at (i, j) */
boolean moveCreature(uint8_t i, uint8_t j) {
 /*
 Serial.println();
 Serial.print("Want to move creature at  row:");
 Serial.print(i);
 Serial.print(" column: ");
 Serial.print(j);
 Serial.println(")");
 */
 if(board[i][j].type != 'w') {
   int r = i, c = j;
   // Plan of moves
   // |-|1|-|
   // |2|x|3|
   // |-|4|-|
   // Positions            1      2      3      4
   uint8_t pos [][2] = { {0,0}, {0,0}, {0,0}, {0,0} };
   pos[0][1] = c;  // for 1 column does not change
   pos[1][0] = r;  // for 2 row does not change
   pos[2][0] = r;  // for 3 row does not change
   pos[3][1] = c;  // for 4 column does not change
   // We are in a torus
   if(r == 0)
     pos[0][0] = 7; // For 1 go to last row
   else
     if(r == 7)
       pos[3][0] = 0; // For 4 go to first row
     else {
       // Normal moves
       pos[0][0] = r - 1;
       pos[3][0] = r + 1; 
     }
   if(c == 0)
     pos[1][1] = 7; // For 2 go to last column
   else
     if(c == 7)
       pos[2][1] = 0; // For 3 go to first column
     else {
       // Normal moves
       pos[1][1] = c - 1;
       pos[2][1] = c + 1; 
     }
   // Behaviours
   // Check space and food
   boolean space = false, fish = false;
   for(uint8_t it = 0 ; it < 4 ; it++){
     if(board[pos[it][0]][pos[it][1]].type == 'w')
       space = true;
     else
       if(board[pos[it][0]][pos[it][1]].type == 'f')
         fish = true;
   }
   int choose = -1;
   // Fish's and shark's behaviour (when no food)
   if(board[i][j].type == 'f' || (board[i][j].type == 's' && !fish)){
     if(space) {
       choose = random(4);
       while(board[pos[choose][0]][pos[choose][1]].type != 'w'){
         choose = random(4);
       }
     }
   } else {
      // Shark's behaviour when there is food
      choose = random(4);
      while(board[pos[choose][0]][pos[choose][1]].type != 'f'){
        choose = random(4); 
      }
   }
   // Update blank spaces and destination places
   if(space || fish) {
      board[pos[choose][0]][pos[choose][1]].type = board[i][j].type;
      board[i][j].type = 'w';
      board[pos[choose][0]][pos[choose][1]].survivedChro = board[i][j].survivedChro;
      board[i][j].survivedChro = 0;
      if(fish && board[pos[choose][0]][pos[choose][1]].type == 's'){
        // The shark has eaten the fish
        board[pos[choose][0]][pos[choose][1]].energy = board[i][j].energy + 1;
      } else {
       board[pos[choose][0]][pos[choose][1]].energy = board[i][j].energy; // Fish's energy = 0 
      }
      board[i][j].energy = 0;
   }
   return space || fish;
 }else return false;
}
void loop() {
 paint();
 delay(500);
 nextChronon();
}

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include "drawing.h"
#include "serial_handling.h"
#include "consts_and_types.h"


#define JOY_VERT  A9 // should connect A9 to pin VRx
#define JOY_HORIZ A8 // should connect A8 to pin VRy
#define JOY_SEL   53
#define JOY_CENTER   512
#define JOY_DEADZONE 64

extern shared_vars shared;
MCUFRIEND_kbv tft;

void setup(){
  init();

  Serial.begin(9600);
  Serial.flush(); // get rid of any leftover bits

  pinMode(JOY_SEL, INPUT_PULLUP);

  shared.tft = &tft;
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  mainmenu();
  drawstartgame();	
}

// Controls current snake direction on client side and sends information to game logic
// through serial monitor
void processJoystick() {
  int xVal = analogRead(JOY_HORIZ);
  int yVal = analogRead(JOY_VERT);
  delay(200);

  // Use logical yVal and xVal values which is determined by how far the joystick is moved in different directions
  // and which way the snake is 
  
  shared.old_direction = shared.direction; // Used for drawing turns in the snake body

  //Goes down
  if (yVal < JOY_CENTER - JOY_DEADZONE) {
  	shared.direction = "AU";
  }
  //Goes up
  else if (yVal > JOY_CENTER + JOY_DEADZONE) {
  	shared.direction = "AD";
  }
  //Goes left
  if (xVal > JOY_CENTER + JOY_DEADZONE) {
  	shared.direction = "AL";
  }
  //Goes right
  else if (xVal < JOY_CENTER - JOY_DEADZONE) {
  	shared.direction = "AR";
  }

  // Checking if the snake turned so that a snake corner can be drawn
  if (shared.old_direction != shared.direction) {
    shared.turnflag = true;
    shared.mycoord[4] = shared.mycoord[0];
    shared.mycoord[5] = shared.mycoord[1];
  }

  // Print the current direction to serial monitor so game logic can read it
  Serial.println(shared.direction);
}

int main(){
	setup();
	while (true){
		processJoystick();
		waitforsigns(); // Waits for game logic feedback about what next to do
	}
}


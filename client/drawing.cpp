#include "drawing.h"
#include "EEPROM.h"

shared_vars shared;

#define JOY_SEL 53
#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 320
#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREEN 0x07E0
#define RED 0xF800

// Draws the default starting state of the actual game
void drawstartgame(){
	(shared.tft)->fillScreen(BLACK);
	(shared.tft)->fillRect(0,0,DISPLAY_WIDTH,20,WHITE);
	(shared.tft)->fillRect(DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2,20,20,GREEN);
	(shared.tft)->fillRect(240,80,20,20,RED);
	(shared.tft)->setCursor(220,10);
	(shared.tft)->setTextSize(1);
	(shared.tft)->setTextColor(RED,WHITE);
	(shared.tft)->print("SNAKE THE GAME");
	(shared.tft)->setCursor(400,10);
	(shared.tft)->print("SCORE: 0");
	shared.score = 0;
}

// Redraws score in top right during game (only called when fruit is eaten)
void drawscore(){
	(shared.tft)->fillRect(430,10,60,10,WHITE);
	(shared.tft)->setCursor(400,10);
	(shared.tft)->print("SCORE: ");
	(shared.tft)->print(shared.score);
}

// Similar to drawing snake, but we draw a fruit elsewhere instead
// of undrawing snake tail
void draweatfruit(){
	// Getting coords
	int draw_coord_x = shared.mycoord[0];
	int draw_coord_y = shared.mycoord[1];
	int fruit_loc_x = shared.mycoord[2];
	int fruit_loc_y = shared.mycoord[3];

	//Draw snake
	(shared.tft)->fillRect(draw_coord_x,draw_coord_y,20,20,GREEN);
	//Draw new fruit
	(shared.tft)->fillRect(fruit_loc_x,fruit_loc_y,20,20,RED);

	//Potentially need to draw a corner if snake turned
	if (shared.turnflag && shared.score > 0) {
		drawturn();
	}

	// Increase score and redraw it
	shared.score++;
	drawscore();
}

// Standard redrawing of the snake
void drawsnake(){
	// Get coords
	int draw_coord_x = shared.mycoord[0];
	int draw_coord_y = shared.mycoord[1];
	int undraw_coord_x = shared.mycoord[2];
	int undraw_coord_y = shared.mycoord[3];

	// Draw head
	(shared.tft)->fillRect(draw_coord_x,draw_coord_y,20,20,GREEN);
	// Undraw tail
	(shared.tft)->fillRect(undraw_coord_x,undraw_coord_y,20,20,BLACK);

	// Potentially need to draw a corner if snake turned
	if (shared.turnflag && shared.score > 0) {
		drawturn();
	}
}

// Draws list of highscores achieved on this arduino
void drawhighscores(){
	// Title
	(shared.tft)->fillScreen(BLACK);
	(shared.tft)->fillRect(0,0,DISPLAY_WIDTH,20,WHITE);
	(shared.tft)->setCursor(180,10);
	(shared.tft)->setTextSize(1);
	(shared.tft)->setTextColor(RED,WHITE);
	(shared.tft)->print("HIGHSCORES");
	(shared.tft)->setTextSize(2);

	// Print the actual highscores
  (shared.tft)->setCursor(0, 30); 
  (shared.tft)->setTextWrap(false);
  int selected = isort_highscores(); // sort highscore before displaying

  // Only print largest 10 out of 11 highsores stored
  for (int i = 10; i>=1; i--){

  		// So if the score achieved is top 10, it is highlighted 
      if (i != selected)
      { // not  highlighted
          //  white  characters  on  black  background
          (shared.tft)->setTextColor(0xFFFF, 0x0000);
      }
      else
      { //  highlighted
          //  black  characters  on  white  background
          (shared.tft)->setTextColor(0x0000, 0xFFFF);
      }

      // Print the score
      (shared.tft)->setCursor(100, abs(i-11)*30);
      (shared.tft)->print(abs(i-11)); (shared.tft)->print(": ");
      (shared.tft)->print(EEPROM[i]);
  }

  // Tells the user to press joystick to play again
  (shared.tft)->setCursor(200,150);
  (shared.tft)->setTextColor(WHITE);
  (shared.tft)->print("Press the joystick");
  (shared.tft)->setCursor(180,170);
  (shared.tft)->print("  to play again");
  (shared.tft)->drawRect(195,145,230,50,GREEN);

  // Waits for user to press joystick
  int buttonVal = digitalRead(JOY_SEL);
  while ((buttonVal) == HIGH){
		buttonVal = digitalRead(JOY_SEL);
	}
}

// Sequence of drawing after the player loses
void drawendgame(){
	(shared.tft)->fillScreen(BLACK);
	(shared.tft)->setTextSize(4);
	(shared.tft)->setCursor(140,140);
	(shared.tft)->setTextColor(WHITE,BLACK);
	(shared.tft)->print("GAME OVER!");
	delay(2000);
	drawhighscores();
	drawstartgame(); 
}

// Reminds user to run game_logic before playing game
void mainmenu(){
	int buttonVal = digitalRead(JOY_SEL);
	(shared.tft)->setTextColor(WHITE,BLACK);
	(shared.tft)->setCursor(130,140);
	(shared.tft)->setTextSize(3);
	(shared.tft)->print("RUN GAME LOGIC!");
	(shared.tft)->setTextSize(2);
	(shared.tft)->setCursor(20,180);
	(shared.tft)->print("PRESS DOWN JOYSTICK TO START THE GAME");
	while ((buttonVal) == HIGH){
		buttonVal = digitalRead(JOY_SEL);
	}
}

// Draws a corner of the snake body if it turned
void drawturn(){
	//Top left coordinates of the old coordinates (where corner is)
	int x = shared.mycoord[4];
	int y = shared.mycoord[5];

	// Four possible directions for the corner to be drawn in
	// Top right corner slightly undrawn
	if ((shared.old_direction.equals("AU") && shared.direction.equals("AL"))||(shared.old_direction.equals("AR") && shared.direction.equals("AD"))){
		(shared.tft)->fillRect(x+5,y,15,5,BLACK);
		(shared.tft)->fillRect(x+15,y,5,15,BLACK);
	}
	// Top left corner slightly undrawn
	else if ((shared.old_direction.equals("AU") && shared.direction.equals("AR"))||(shared.old_direction.equals("AL") && shared.direction.equals("AD"))){
		(shared.tft)->fillRect(x,y,15,5,BLACK);
		(shared.tft)->fillRect(x,y,5,15,BLACK);
	}
	// Bottom right corner slightly undrawn
	else if ((shared.old_direction.equals("AD") && shared.direction.equals("AL"))||(shared.old_direction.equals("AR") && shared.direction.equals("AU"))){
		(shared.tft)->fillRect(x+15,y+5,5,15,BLACK);
		(shared.tft)->fillRect(x+5,y+15,15,5,BLACK);
	}
	// Bottom left corner slightly undrawn
	else if ((shared.old_direction.equals("AD") && shared.direction.equals("AR"))||(shared.old_direction.equals("AL") && shared.direction.equals("AU"))){
		(shared.tft)->fillRect(x,y+5,5,15,BLACK);
		(shared.tft)->fillRect(x,y+15,15,5,BLACK);
	}
	shared.turnflag = false; // Reset the turn flag
}

// Insertion sort of the highscores (notes below function)
// (Not a drawing function, but we felt like it belonged here)
int isort_highscores() {
    int recent_score = EEPROM[0];

    //Sort
    int j;
    int i = 1;
    while (i < 11)
    {
        j = i;
        while ((j > 0) & (EEPROM[j - 1] > EEPROM[j]))
        {
            // Swap
            int temp = EEPROM[j];
            EEPROM[j] = EEPROM[j-1];
            EEPROM[j-1] = temp;
            j--;
        }
        i++;
    }

    // Find the new index of recent score
    int idx = 0;
    for (int i = 0; i<11; i++) {
        if (EEPROM[i] == recent_score && i>0) {
            idx = i;
        }
    }
    return idx;
}
// The lastest score achieved will be put in index 0 (overwriting the lowest out of the 11 stored),
// but only indeces 10 to 1 will be displayed. That way, if the score was not within the top 10 score, 
// it will not be displayed. Its index will be returned to help with highlighting the most recently achieved score.
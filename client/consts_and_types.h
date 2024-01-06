#ifndef __CONSTS_AND_TYPES_H
#define __CONSTS_AND_TYPES_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

struct shared_vars{
	// [snake head x, snake head y, snake tail x, snake tail y, right behind head x, right behind head y]
	int mycoord[6];

	// tft display
  MCUFRIEND_kbv* tft;

  // Current score of the game in progress (how many fruits have been eaten)
  int score;

  // Current direction of the snake
  String direction = "AU";

  // Direction of snake in last game tick (used in detecting if snake turned)
  String old_direction = "AU";
  bool turnflag = false; // set to true if snake turned
};


#endif
# Arduino Snake Game

Final project for CMPUT 275 Winter 2022 semester

## Info
* **Names**: 	Rowan Tilroe, Filippo Ciandy
* **IDs**: 	1629172, 1641346
* **Semester**:  CMPUT 275 WN2020

## Included files:
* game_logic (desktop):
  * game_logic.cpp
  * snake.h, snake.cpp
  * serialport.cpp, serialport.h
  * Makefile
* client (arduino):
  * Makefile
  * client.cpp
  * consts_and_types.h
  * drawing.h, drawing.cpp
  * EEPROM.h
  * serial_handling.h, serial_handling.cpp
  * README

## Accessories:
* Arduino
* Breadboard
* Joystick
* Wires
* TFT display
* USB cable

## Wiring Instructions:
* Pin A8 <---> VRy on joystick
* Pin A9 <---> VRx on joystick
* GND <---> GRD on joystick
* 5V <---> 5V on joystick
* Digital Pin 53 <---> SW on joystick

Running instructions:
	1. Open two terminals, one in the game_logic folder and one in the client folder
	2. In either terminal type "arduino-port-select" and enter two "1"s to connect the Arduino
	3. In the client folder, type "make upload" to upload client.cpp to the arduino
	4. Once client.cpp is uploaded to the arduino, type "make" followed by "./game_logic" to start up the game logic
	side of the game
	5. Press the joystick down to begin the game. The joystick can then control the direction of the snake

## Notes & Assumptions:
* General comments:
  * The basic idea behind the function of the game is to let a desktop program take care of the game logic
    and send instructions about what to draw and where to the arduino through the serial port. This was mainly done
    to gain access to the queue and unordered_map data structures to constant game updates.

* game_logic:
  * The snake_class has a body queue that keeps track of the location of each of the 20x20 blocks that make up the
		snake's body, where the back of the queue is the head and the front is its tail. It also has a unordered map 
		that keeps track of the entire grid of the game with the coordinates as the key and a character representing
		the type of tile it is as the value (E = empty, S = snake, F = fruit). Lastly, the snake has a charater that represents
		the current direction it is heading.
		* Game updates can be broken down into a few phases. First, we read the direction of the snake from the serial port and
		update it. Then, we get the next tile if front of the snake based on its direction. We check that tile for 3 possible "events":
		Game over, move snake, or snake eat. Based on that assessment of the next tile, we construct a string to send back to the arduino
		that contains information about what to do next.
* client:
  * In the serial handling file, lines of symbols and/or coordinates which are seperated by spaces are sent by the server to the client, the client reads the first character which can either be (G,F or E) and parses the line according the character read in.
  * In the file drawing.cpp, according to the previous and current direction of the snake the drawturn function redraws rectangles which has the same colour as the background.
  * The EEPROM is used to store the highscores. That way, when the user turns off the arduino, the highscores are still saved.
  * We choose to display the top 10 highscores. However, the EEPROM has a size of 11. The reason for this is because we stored the most recently aquired score in index 0. The EEPROM is then sorted with insertion sort. Then, indeces 10 through 1 are displayed on the screen leaving out the smallest score in index 0 which is to be overwritten the next time the user plays the game.  

## Makefile targets (game_logic):
* game_logic: the main executable file of game_logic.cpp
* game_logic.o: creates an object file of game_logic.o
* snake.o: creates an object file of snake.cpp
* serial_port.o creates an object file of serial_port.cpp
* clean: removes all of the above object and executable files

## Task Breakdown:
* Rowan Tilroe:
  * snake_class implementation
  * Serial handling on server side
  * Highscore using the EEPROM implementation

* Filippo Ciandy:
  * Serial handling on client side
  * Drawing functions
  * Checking when snake turned to draw snake corner

## Unimplemented Features:
* We planned on adding a feature to input a name whenever a highscore was achieved. However this was before we planned
	on using the serial monitor for game control. Because of the way we implemented game control, we could no longer use
	the serial montior to enter the user's name like we planned. In hindsight, desktop input could have been a possibility, but
	it would likely be very awkward to implement.

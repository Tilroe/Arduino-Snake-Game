#include "snake.h"
#include "serialport.h"
#include <iostream>
using namespace std;

int main() {
	// Setup -----------------------------------------------------

	// Creating an instance of the SerialPort class
	SerialPort Serial("/dev/ttyACM0");

	// Initialize game
	snake_class snake;

	// Game loop -------------------------------------------------
	while (true) {
		// Read the direction of the snake and update it
		string line = Serial.readline();
		cout << "Recieved: " << line;
		snake.chg_direction(line);

		// Compute next instruction for arduino to perform
		string instruction = snake.update();
		cout << "Sent: " << instruction;
		Serial.writeline(instruction);
	}
}
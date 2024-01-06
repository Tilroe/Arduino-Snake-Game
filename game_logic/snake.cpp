#include "snake.h"
#include <cstdlib> // for rand
using namespace std;

void snake_class::reset() {
	// Initialize grid
	for (int x = 0; x <= 460; x += 20) {
		for (int y = 20; y <= 300; y += 20) {
			xy_pos point;
			point.x = x;
			point.y = y;

			// The game will start with the snake at (240, 160) with a fruit at (240, 80)
			if (x == 240 && (y == 160 || y == 80)) {
				if (y == 160) {
					grid[point] = 'S'; // for "snake"
				}
				else if (y == 80) {
					grid[point] = 'F'; // for "fruit"
				}
			}
			else {
				grid[point] = 'E'; // for "empty"
			}
		}
	}

	// Initialize snake
	while (!body.empty()) {
		body.pop();
	}
	xy_pos point;
	point.x = 240;
	point.y = 160;
	body.push(point);
	direction = 'u';
}

snake_class::snake_class() {
	reset();
}

void snake_class::chg_direction(string dir) {
	if (dir.compare("AU\n") == 0) {
		direction = 'u';
	}
	else if (dir.compare("AD\n") == 0) {
		direction = 'd';
	}
	else if (dir.compare("AL\n") == 0) {
		direction = 'l';
	}
	else {
		direction = 'r';
	}
}

xy_pos snake_class::get_ahead() {
	xy_pos head = body.back();
	xy_pos ahead = head;

	// Find the next tile
	if (direction == 'u') {
		ahead.y -= 20;
	}
	else if (direction == 'd') {
		ahead.y += 20;
	}
	else if (direction == 'l') {
		ahead.x -= 20;
	}
	else {
		ahead.x += 20;
	}
	return ahead;
}

char snake_class::check_tile(xy_pos ahead) {
	// Check if out of bounds
	if (ahead.y >= 320 || ahead.y < 20 || ahead.x >= 480 || ahead.x < 0) {
		return 'E';
	}

	// Check if snake collided with itself
	else if (grid[ahead] == 'S') {
		return 'E';
	}

	// Check if touched fruit
	else if (grid[ahead] == 'F') {
		return 'F';
	}

	// Otherwise, no events happened
	else {
		return 'G';
	}
}

pair<xy_pos, xy_pos> snake_class::move(xy_pos new_head) {
	// Push new head on snake body, get old tail
	body.push(new_head);
	xy_pos old_tail = body.front();

	// Update grid
	grid[new_head] = 'S';
	grid[old_tail] = 'E';

	// Remove old tail from body
	body.pop();

	// Return (new head, old tail)
	return make_pair(new_head, old_tail);
}

pair<xy_pos, xy_pos> snake_class::eat(xy_pos new_head) {
	// Push new head on snake body, keep old tail
	body.push(new_head);

	// Update grid
	grid[new_head] = 'S';

	// Find new spot for fruit
	bool good_spot = false;
	xy_pos fruit;

	while (!good_spot) {
		// Generate fruit
		int x = rand() % 24; // between 0 and 23
		int y = rand() % 15 + 1; // between 1 and 15
		x = x*20; // between 0 and 460
		y = y*20; // between 20 and 300
		fruit.x = x;
		fruit.y = y;

		// Check if not inside snake
		if (grid[fruit] != 'S') {
			grid[fruit] = 'F';
			good_spot = true;
		}
	}

	// Return (new head, fruit)
	return make_pair(new_head, fruit);
}

string snake_class::update() {
	// Check the next tile
	xy_pos ahead = get_ahead();
	char event = check_tile(ahead);

	string instruction; // to be sent to the arduino, constructed below
	// Different events:

	// E: End game
	if (event == 'E') {
		instruction = "E\n";
		this->reset();
	}

	// F: Ate fruit
	else if (event == 'F') {
		pair<xy_pos, xy_pos> head_and_fruit = eat(ahead);
		instruction = "F " + to_string(head_and_fruit.first.x) + " " + to_string(head_and_fruit.first.y) + " " 
											 + to_string(head_and_fruit.second.x) + " " + to_string(head_and_fruit.second.y) + "\n";
	}

	// G: No event, just move
	else {
		pair<xy_pos, xy_pos> head_and_tail = move(ahead);
		instruction = "G " + to_string(head_and_tail.first.x) + " " + to_string(head_and_tail.first.y) + " "
											 + to_string(head_and_tail.second.x) + " " + to_string(head_and_tail.second.y) + "\n";
	}

	return instruction;
}
#include <queue>
#include <unordered_map>
#include <utility>
#include <string>

using namespace std;

// Used a lot in snake class. Also used as key in unordered map so hashing and
// comparison need to be defined
struct xy_pos {
	int x;
	int y;

	// Defining '==' xy_pos
	bool operator == (const xy_pos & other_pos) const {
		return (x == other_pos.x) && (y == other_pos.y);
	}
};

// Specializing the hashing of a xy_pos
namespace std {
	template<>
	struct hash<xy_pos> {
		size_t operator() (const xy_pos & point) const {
			return point.x + point.y;
		}
	};
}

class snake_class {
public:
	snake_class(); // constructor
	//~snake_class(); // destructor
	

	void reset(); // Reset game state
	void chg_direction(string dir); // Change the direction 
	xy_pos get_ahead(); // Get the tile one square ahead of the snake 
	char check_tile(xy_pos); // Check the tile for possible events
	pair<xy_pos, xy_pos> eat(xy_pos new_head); // Call when snake moves & eats fruit 
	pair<xy_pos, xy_pos> move(xy_pos new_head); // Call when snake moves
	string update(); // Performs regular game update and returns string to send to arduino

private:
	queue<xy_pos> body; // models the snake (front of the queue = back of snake)
	unordered_map<xy_pos, char> grid; // models the entire grid
	char direction = 'u';
};
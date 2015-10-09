#ifndef COMPASS
#define COMPASS
	enum Direction = {NORTH, EAST, SOUTH, WEST}
	bool isDirection(string input) {
		for (int i = 0; i < Direction.size(); i++) {
			if (Direction[i] == input) {
				return true;
			}
		}
		return false;
	}
#endif
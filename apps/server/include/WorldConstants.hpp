#ifndef WORLD_COMPASS
#define WORLD_COMPASS
#include <iostream>
#include <string>
#include <vector>



namespace WorldConstants {
	const std::vector<std::string> possibleDirections = {"NORTH", "EAST", "SOUTH", "WEST", "UP", "DOWN"};
	bool isDirection(std::string input) {
		for (int i = 0; i < possibleDirections.size(); i++) {
			if (possibleDirections[i] == input) {
				return true;
			}
		}
		return false;
	}
}
#endif

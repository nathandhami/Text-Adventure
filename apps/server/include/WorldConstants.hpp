#ifndef WORLD_COMPASS
#define WORLD_COMPASS
#include <iostream>
#include <string>

using namespace std;

const string possibleDirections[] = {"NORTH", "EAST", "SOUTH", "WEST", "UP", "DOWN"};

bool isDirection(string input) {
	for (int i = 0; i < possibleDirections->size(); i++) {
		if (possibleDirections[i] == input) {
			return true;
		}
	}
	return false;
}
#endif

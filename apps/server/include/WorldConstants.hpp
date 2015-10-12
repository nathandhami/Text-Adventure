#ifndef WORLD_COMPASS
#define WORLD_COMPASS
#include <iostream>
#include <string>

using namespace std;

static const string possibleDirections[] = {"NORTH", "EAST", "SOUTH", "WEST", "UP", "DOWN"};

static bool isDirection(string input) {
	for (int i = 0; i < possibleDirections->size(); i++) {
		if (possibleDirections[i] == input) {
			return true;
		}
	}
	return false;
}
#endif

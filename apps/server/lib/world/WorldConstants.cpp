#include "WorldConstants.hpp"

bool WorldConstants::isDirection(std::string input) {
	const std::vector<std::string> possibleDirections = {"NORTH", "EAST", "SOUTH", "WEST", "UP", "DOWN"};
	for (int i = 0; i < possibleDirections.size(); i++) {
		if (possibleDirections[i] == input) {
			return true;
		}
	}
	return false;
}
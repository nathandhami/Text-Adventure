#include "WorldConstants.hpp"

bool WorldConstants::isDirection(std::string input) {
	const std::vector<std::string> possibleDirections = {"NORTH", "NORTHWEST", "NORTHEAST", "EAST", "SOUTH", "SOUTHWEST", "SOUTHEAST", "WEST", "UP", "DOWN"};

	boost::replace_all(input, " ", "");
	boost::to_upper(input);
	if (std::find(possibleDirections.begin(), possibleDirections.end(), input) != possibleDirections.end()) {
		return true;
	}
	return false;
}
#include <iostream>
#include <CommandParser.hpp>
#include <DatabaseTool.hpp>

int main() {
	DatabaseTool::putCharInZone(1, 3054);
	cout << CommandParser::handleIDandCommand(1, "north") << endl;
	return 1;
}
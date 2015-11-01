#include <iostream>
#include "CommandParser.hpp"
#include "DatabaseTool.hpp"
#include "DictionaryCmds.hpp"

int main() {
	//DatabaseTool::putCharInZone(1, 3054);
	std::tuple<enum CommandHeader, Command> output = (CommandParser::getHeaderAndCommand(" fight "));
	enum CommandHeader header = get<0>(output);
	std::string strheader;
	if(header == WORLD){
		strheader = "WORLD";
	}else if (header == COMBAT){
		strheader = "COMBAT";
	}else if (header == MESSENGER){
		strheader = "MESSENGER";
	}else if (header == INCOMPLETE){
		strheader = "INCOMPLETE";
	}else if (header == INVALID){
		strheader = "INVALID";
	}else if (header == CHARACTER){
		strheader = "CHARACTER";
	}else if (header == EDITOR){
		strheader = "EDITOR";
	}
	Command parsedcommand = get<1>(output);
	cout << "Header = " + strheader << endl;
	cout << "Command Type = " + parsedcommand.type << endl;
	cout << "Command Data = " + parsedcommand.data << endl;
	return 0;
}
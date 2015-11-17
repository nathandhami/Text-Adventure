#include <iostream>
#include "CommandParser.hpp"
#include "DatabaseTool.hpp"
#include "DictionaryCmds.hpp"

using namespace CommandHeader;

int main() {
	//DatabaseTool::putCharInZone(1, 3054
	std::string str = "fight";
	std::tuple<int, Command> output = (CommandParser::getHeaderAndCommand(str));
	int header = get<0>(output);
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
	
	
	output = (CommandParser::getHeaderAndCommand(str));
	header = get<0>(output);
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
	parsedcommand = get<1>(output);
	cout << "Header = " + strheader << endl;
	cout << "Command Type = " + parsedcommand.type << endl;
	cout << "Command Data = " + parsedcommand.data << endl;
	
	
	return 0;
}
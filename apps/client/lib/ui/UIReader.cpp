#include "UIReader.hpp"
#include "ConsoleReader.hpp"

#include <iostream>


std::string UIReader::getUserName() {
	return ConsoleReader::getInlineInput( "Username: " );
}

std::string UIReader::getPassword() {
	return ConsoleReader::getInlineInput( "Password: " );
}
	
std::string UIReader::getCommand() {
	return ConsoleReader::getInlineInput( "> " );
}
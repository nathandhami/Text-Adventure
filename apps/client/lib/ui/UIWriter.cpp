#include "UIWriter.hpp"
#include "ConsoleWriter.hpp"

#include <iostream>

void UIWriter::sendSysStatus( std::string message ) {
	ConsoleWriter::write( message );
}
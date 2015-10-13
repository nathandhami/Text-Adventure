#include "ConsoleWriter.hpp"



void ConsoleWriter::write( std::string message ) {
	std::cout << message << std::endl;
}


void ConsoleWriter::writeInline( std::string message ) {
	std::cout << message;
}
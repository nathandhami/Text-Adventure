#include "ConsoleReader.hpp"
#include "ConsoleWriter.hpp"

#include <string>


std::string ConsoleReader::getInput( std::string message ) {
	
}


std::string ConsoleReader::getInlineInput( std::string message ) {
	std::string inputString;
	do {
		ConsoleWriter::writeInline( message );
    	getline( std::cin, inputString );
		if ( inputString.empty() ) {
			ConsoleWriter::write( "Cannot be empty, please try again." );
		}
  	} while( inputString.empty() );
	
	return inputString;
}
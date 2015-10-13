#ifndef UIREADER_H
#define UIREADER_H

#include <iostream>

namespace UIReader {
	
	std::string getUserName();
	std::string getPassword();
	
	// Blocking
	std::string getCommand();
	
}


#endif
#ifndef UIREADER_HPP
#define UIREADER_HPP

#include <iostream>

namespace UIReader {
	
	std::string getUserName();
	std::string getPassword();
	
	// Blocking
	std::string getCommand();
	
}


#endif
#ifndef COMMANDER_HPP
#define COMMANDER_HPP


#include <string>
#include <utility>

#include "User.hpp"



namespace Commander {
	
	std::pair< std::string, std::string > handleCommand( User user, std::string commandText );
	
}


#endif
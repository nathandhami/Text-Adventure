#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

//********************************************************************** 
// AUTHENTICATOR
// 
// This module serves as a middleware between the client and their 
// ability to interact with the world and database.
//
// Author: Pavel Kozlovsky (pkozlovs@sfu.ca)
//**********************************************************************

#include <iostream>
#include <utility>

#include "User.hpp"

class Authenticator {
public:
	static bool login( User& user, std::string userCredentials );
	static bool logout( User& user );
	
	static std::pair< std::string, std::string > registerAccount( std::string userCredentials );
	
private:
	Authenticator() {}
	
};



#endif

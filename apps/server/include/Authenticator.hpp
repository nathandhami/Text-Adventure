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

#include "User.hpp"

class Authenticator {
public:
	static int login( std::string data );
	static bool login( User& user, std::string userCredentials );
	static void logout( int userId );
	static bool logout( User& user );
	
private:
	Authenticator() {}
	
};



#endif

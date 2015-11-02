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

namespace Authenticator {
	int login( std::string data );
	bool login( User& user, std::string userCredentials );
	void logout( int userId );
	bool logout( User& user );
}



#endif

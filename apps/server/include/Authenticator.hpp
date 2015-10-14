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

namespace Authenticator {
	int login( std::string data );
	void logout( int userId );
}

#endif

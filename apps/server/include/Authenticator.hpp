#ifndef AUTHENTICATOR_HPP
#define AUTHENTICATOR_HPP

#include <iostream>

namespace Authenticator {
	int login( std::string data );
	void logout( int userId );
}


#endif
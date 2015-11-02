#include "Authenticator.hpp"
#include "DatabaseTool.hpp"

#include <boost/algorithm/string.hpp>
#include <vector>




bool Authenticator::login( User& user, std::string data ) {
	const int NO_USER_ID = 0;
	
	std::vector< std::string > credentials;
	boost::split( credentials, data, boost::is_any_of( ";" ) );
	
	std::cout << "[Auth] User's credentials: " << std::endl;
	std::cout << "[Auth] \tUsername: " << credentials[ 0 ] << std::endl;
	std::cout << "[Auth] \tPassword: " << credentials[ 1 ] << std::endl;
	
	bool signedIn = DatabaseTool::signUserIn( credentials[ 0 ], credentials[ 1 ] );

	if ( signedIn ) {
		user.setUserId( DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] ) );
		std::cout << "[Auth] Logged: " << DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] ) << std::endl;
	} else {
		std::cout << "[Auth] Failed to log in." << std::endl;
	}
		
	return signedIn;
}


bool Authenticator::logout( User& user ) {
	const int NO_USER_ID = 0;
	
	if ( user.isAuthorized() ) {
		// TO-DO: reimplement once character selection becomes a thing
		
		bool signedOut = DatabaseTool::signUserOut( user.getUserId() );

		if ( signedOut ) {
			std::cout << "[Auth] Logged out." << std::endl;
			DatabaseTool::setCharOffline( user.getUserId() );
			user.setUserId( NO_USER_ID );
		} else {
			std::cout << "[Auth] Failed to log out." << std::endl;
		}
		
		return signedOut;
	}
	
	return false;
}


void Authenticator::logout( int userId ) {
	if ( userId ) {
		DatabaseTool::setCharOffline( userId );
	}
}

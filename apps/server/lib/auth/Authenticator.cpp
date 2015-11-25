#include "Authenticator.hpp"
#include "DatabaseTool.hpp"
#include <GameCode.hpp>

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
		user.userId = DatabaseTool::getUserID( credentials[ 0 ], credentials[ 1 ] );
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
			user.userId = NO_USER_ID;
		} else {
			std::cout << "[Auth] Failed to log out." << std::endl;
		}
		
		return signedOut;
	}
	
	return false;
}


std::pair< std::string, std::string > Authenticator::registerAccount( std::string userCredentials ) {
	const int NUM_EXP_ARGS = 3;
	
	std::vector< std::string > credentials;
	boost::split( credentials, userCredentials, boost::is_any_of( ";" ) );
	if ( credentials.size() != NUM_EXP_ARGS ) return std::make_pair( GameCode::INVALID, "Client error detected. Please redownload your client." );
	
	std::string userName = credentials[ 0 ];
	std::string password = credentials[ 1 ];
	std::string passwordRep = credentials[ 2 ];
	if ( userName.empty() || password.empty() ) return std::make_pair( GameCode::INVALID, "Required fields are empty." );
	if ( DatabaseTool::userExists( userName ) ) return std::make_pair( GameCode::INVALID, "User with this name already exists." );
	if ( password != passwordRep ) return std::make_pair( GameCode::INVALID, "Passwords do not match." );
	
	if ( !DatabaseTool::addUser( userName, password ) ) return std::make_pair( GameCode::ERROR, "Internal server error occurred." );
	
	return std::make_pair( GameCode::OK, "You have successfully registered!" );
}
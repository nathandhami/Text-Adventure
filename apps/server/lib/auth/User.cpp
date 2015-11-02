#include "User.hpp"


bool User::isAuthorized() const {
	const int NO_USER_ID = 0;
	return ( this->userId != NO_USER_ID );
}


void User::setUserId( int userId ) {
	this->userId = userId;
}


int User::getUserId() const {
	return this->userId;
}


void setSelectedCharacterId( int characterId ) {
	
}
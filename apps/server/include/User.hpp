#ifndef USER_HPP
#define USER_HPP


class User {
friend class Authenticator;
friend class CharacterManager;

public:
	
	User() {}
	
	bool isAuthorized() const;
	int getUserId() const;
	int getSelectedCharacterId() const;
	
private:
	int userId = 0;
	int selectedCharacterId = 0;
	
};


#endif
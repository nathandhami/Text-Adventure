#ifndef USER_HPP
#define USER_HPP


class User {
public:
	friend class Authenticator;
//	friend class CharacterManager;
	
	User() {}
	
	bool isAuthorized() const;
	int getUserId() const;
	void getSelectedCharacterId() const;
	
private:
	int userId = 0;
	int selectedCharacterId = 0;
	
};


#endif
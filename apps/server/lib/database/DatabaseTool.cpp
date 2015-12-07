#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <sstream>
#include "DatabaseTool.hpp"
#include "sqlite_modern_cpp.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <mutex> 

const string DB_LOCATION = "apps/server/databases/adventureDB";
const string INITIAL_ZONE = "3054";
const string FOREIGN_KEY_ON = "PRAGMA foreign_keys = ON;";
const int PLAYER_OFFLINE = 0;
const int PLAYER_ONLINE = 1;
const int INVALID_COMMAND = 4;

static std::mutex databaseMutex;

const int verbosity = 0;

using namespace std;
using namespace sqlite;




string DatabaseTool::quotesql( const string& s ) {

	if(s.empty()) {
		return string("NULL");
	} else {
    	return string("'") + s + string("'");
	}
}

bool DatabaseTool::addUser(string userName, string password) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "INSERT INTO users (userID, userName, password, authencationLevel, signedOn) VALUES ( NULL, ?, ?, 0, 0);"
		<<userName
		<<password;

		databaseMutex.unlock();
		return true;
	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

int DatabaseTool::getUserID(string userName, string password){
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );
		int charID;

		db << "select userID from users where userName= ? AND password= ?;"
		<<userName
		<<password
		>>charID;

		databaseMutex.unlock();
		return charID;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

int DatabaseTool::getUserAuthencationLevel(int userID) {
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );
		int authencationLevel;

		db << "select authencationLevel from users where userID=?"
		<<userID
		>>authencationLevel;

		databaseMutex.unlock();
		return authencationLevel;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

bool DatabaseTool::setUserAuthencationLevel(int userID, int authencationLevel) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "update users SET authencationLevel = ? where userID = ?;"
		<< authencationLevel
		<<userID;

		databaseMutex.unlock();
		return true;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;

	}
}

string DatabaseTool::getPassword(int userID) {
	try {
		databaseMutex.lock();
		string password = "";
		database db( DB_LOCATION );

		db << "select password from users where userID=?;"
		<<userID
		>>password;

		databaseMutex.unlock();
		return password;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}

}

/*bool DatabaseTool::addCharacter(string name, int userID, string description){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;
		db << "INSERT INTO characters (charID, name, userID, location, description) VALUES ( NULL, ? , ?, ?, ?);"
		<<name
		<<userID
		<<INITIAL_ZONE
		<<description;
		int charID = db.last_insert_rowid();

		db << "INSERT INTO playerAttributes VALUES (?, 1, 0, 100, 100, 100, 100, 100, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0);"
		<<charID;

		databaseMutex.unlock();
		return true;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}*/

//Starting item

/*

INSERT INTO `items`(`itemID`,`shortDescription`,`description`,`longDescription`,`keywords`,`isPickable`,`isEquippable`,`isStackable`,`isContainer`) VALUES (NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
UPDATE `items` SET `shortDescription`='a cotton robe' WHERE `_rowid_`='10007';
UPDATE `items` SET `description`='You see an old cotton robe' WHERE `_rowid_`='10007';
UPDATE `items` SET `longDescription`='The old cotton robe looks like it belonged to somebody' WHERE `_rowid_`='10007';
UPDATE `items` SET `keywords`='cotton robe priest' WHERE `_rowid_`='10007';
UPDATE `items` SET `isPickable`=1 WHERE `_rowid_`='10007';
UPDATE `items` SET `isEquippable`=1 WHERE `_rowid_`='10007';
UPDATE `items` SET `isStackable`=0 WHERE `_rowid_`='10007';
UPDATE `items` SET `isContainer`=0 WHERE `_rowid_`='10007';

*/


bool DatabaseTool::addCharacter(string name, int userID, string description){
	try {
		const int STARTING_ITEM_ID = 10007;
		const string STARTING_ITEM = "a cotton robe";
		
		databaseMutex.lock();
		database db(DB_LOCATION);
		
		db	<< FOREIGN_KEY_ON;
		db 	<< "INSERT INTO characters (name, userID, location, description) VALUES (?, ?, 3054, ?);"
			<< name
			<< userID
			<< description;
		
		int charID = db.last_insert_rowid();
		
		db	<< "INSERT INTO playerAttributes VALUES (?, 1, 0, 100, 100, 100, 100, 100, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0);"
			<< charID;
		
		db	<< "INSERT INTO player_inventory (charID, itemID, quantity, isEquipped) VALUES (?, ?, 1, 0)"
			<< charID
			<< STARTING_ITEM_ID;
		
		databaseMutex.unlock();
		
		DatabaseTool::equipItem( charID, STARTING_ITEM );
		return true;
	} catch (sqlite_exception e) {
		databaseMutex.unlock();
		
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		return false;
	}
}

bool DatabaseTool::removeCharacter(string name){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);
		
		db	<< FOREIGN_KEY_ON;
		db	<< "DELETE FROM characters WHERE name = ?;"
			<< name;
		
		databaseMutex.unlock();
		return true;
	} catch ( sqlite_exception e ) {
		databaseMutex.unlock();
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		return false;
	}
}

string DatabaseTool::getCharacterDescription(int charID) {
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );

		string description;

		db << "select description from characters where userID=?;"
		<<charID
		>>description;

		databaseMutex.unlock();
		return description;
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}

}

vector<string> DatabaseTool::getCharactersNames(int userID){
	vector<string> names;
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );

		db << "select name from characters where userID=?;"
		<<userID
		>>[&](string name) {
			names.push_back(name);
		};

		databaseMutex.unlock();
		return names;
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return names;
	}
}
		
int DatabaseTool::getCharIDFromName(string name){
	try {
		databaseMutex.lock();
		int charID = 0;
		database db( DB_LOCATION );

		db << "select charID from characters where name=?"
		<<name
		>>charID;

		databaseMutex.unlock();
		return charID;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

int DatabaseTool::getCharIDInZoneFromName(string name, int zoneID) {
	try {
		databaseMutex.lock();
		int charID = 0;
		database db( DB_LOCATION );

		db << "select X.charID from characters X, charactersOnline Y where X.charID = Y.charID and name=? and location = ?"
		<<name
		<<zoneID
		>>charID;

		databaseMutex.unlock();
		return charID;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

string DatabaseTool::getCharNameFromID(int charID) {
	try {
		databaseMutex.lock();
		string name = "";
		database db(DB_LOCATION);

		db << "select name from characters where charID = ?;"
		<< charID
		>>name;

		databaseMutex.unlock();
		return name;
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}

}

bool DatabaseTool::isCharOnline(int charID){
	try {
		databaseMutex.lock();
		int onlineStatus = 0;
		database db( DB_LOCATION );

		db << "SELECT EXISTS(SELECT 1 FROM charactersOnline WHERE charID= ? LIMIT 1);"
		<<charID
		>>onlineStatus;

		databaseMutex.unlock();
		return onlineStatus;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

bool DatabaseTool::setCharOnline(int charID, string sessionID){
	try{
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;
		db << "INSERT INTO charactersOnline VALUES ( ?, ? , 0);"
		<< charID
		<< sessionID;

		databaseMutex.unlock();
		return true;

	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

bool DatabaseTool::setCharOffline(int charID){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "DELETE FROM charactersOnline WHERE charID = ?;"
		<<charID;

		databaseMutex.unlock();
		return true;

	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

string DatabaseTool::getSessionID(int charID) {
	try {
		databaseMutex.lock();
		string sessionID;
		database db( DB_LOCATION );

		db << "select sessionID from charactersOnline where charID=?;"
		<<charID
		>>sessionID;

		databaseMutex.unlock();
		return sessionID;
	}catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() <<std::endl;
		}

		databaseMutex.unlock();
		return "";
	}

}

int DatabaseTool::getCharID(int userID){
	try {
		databaseMutex.lock();
		int charID = 0;
		database db( DB_LOCATION );

		db << "select charID from characters where userID=?;"
		<<userID
		>>charID;

		databaseMutex.unlock();
		return charID;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;;
		}

		databaseMutex.unlock();
		return 0;
	}
}

bool DatabaseTool::putCharInZone(int charID, int zoneID){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;
		db << "UPDATE characters SET location = ? WHERE charID = ?;"
		<<zoneID
		<<charID;

		databaseMutex.unlock();
		return true;

	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

int DatabaseTool::getCharsLocation(int charID){
	try {
		databaseMutex.lock();
		int zoneID = 0;
		database db( DB_LOCATION );

		db<< "select location from characters where charID=?;"
		<<charID
		>>zoneID;

		databaseMutex.unlock();
		return zoneID;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

vector<int> DatabaseTool::getAllOnlineCharsInZone(int zoneID){
	databaseMutex.lock();
	vector<int> charsInZone;
	database db( DB_LOCATION );

	db << "select x.charID from characters C, charactersOnline X where location= ? AND x.charID = c.charID;"
	<< zoneID
	>>[&](int charID) {
		charsInZone.push_back(charID);
	};

	databaseMutex.unlock();
	return charsInZone;
}

bool DatabaseTool::createNpcInstance(int npcID, int zoneID){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;
		db << "INSERT INTO instanceOfNpc VALUES ( NULL, ?, ?, 1 , 0);"
		<<npcID
		<<zoneID;

		int npcInstanceID = db.last_insert_rowid();

		db << "INSERT INTO npcAttributes VALUES (?, 1, 0, 100, 100, 100, 100, 100, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0);"
		<<npcInstanceID;

		databaseMutex.unlock();
		return true;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

vector<int> DatabaseTool::getAllAliveNpcsInZone(int zoneID){
	databaseMutex.lock();
	vector<int> npcsInZone;
	database db( DB_LOCATION );

	db << "select npcInstanceID from instanceOfNpc where zoneID=? and isAlive = 1;"
	<< zoneID
	>>[&](int npcID) {
		npcsInZone.push_back(npcID);
	};

	databaseMutex.unlock();
	return npcsInZone;
}

bool DatabaseTool::deleteNpcInstance(int npcInstanceID){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "delete from instanceOfNpc where npcInstanceID= ?;"
		<< npcInstanceID;

		databaseMutex.unlock();
		return true;
	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return true;
	}
}

bool DatabaseTool::isNpcAlive(int npcInstanceID){
	try {
		databaseMutex.lock();
		int isAlive;
		database db(DB_LOCATION);

		db << "select isAlive from instanceOfNpc where npcInstanceID = ?;"
		<< npcInstanceID
		>> isAlive;

		databaseMutex.unlock();
		return isAlive;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}

}

bool DatabaseTool::respawnAll(){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "UPDATE instanceOfNpc SET isAlive = 1;";

		databaseMutex.unlock();
		return true;

	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

bool DatabaseTool::murderNpc(int npcInstanceID){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "UPDATE instanceOfNpc SET isAlive = 0 where npcInstanceID = ?;"
		<< npcInstanceID;

		databaseMutex.unlock();
		return true;

	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

bool DatabaseTool::reviveNpc(int npcInstanceID){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "UPDATE instanceOfNpc SET isAlive = 1 where npcInstanceID = ?;"
		<<npcInstanceID;

		databaseMutex.unlock();
		return true;

	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

string DatabaseTool::getNpcDesc(int npcInstanceID){
	try {
		databaseMutex.lock();
		string description;
		database db( DB_LOCATION );

		db << "select description from npcs X, instanceOfNpc Y where X.npcID = Y.npcID and npcInstanceID=?;"
		<<npcInstanceID
		>>description;

		databaseMutex.unlock();
		return description;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}
}

string DatabaseTool::getNpcName(int npcInstanceID){
	try {
		databaseMutex.lock();
		string name;
		database db( DB_LOCATION );

		db << "select shortDesc from npcs X, instanceOfNpc Y where X.npcID = Y.npcID and npcInstanceID=?;"
		<<npcInstanceID
		>>name;

		databaseMutex.unlock();
		return name;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}
}

int DatabaseTool::getNpcIDFromInstanceID(int npcInstanceID) {
	try {
		databaseMutex.lock();
		int npcID;
		database db( DB_LOCATION );

		db << "select npcID from instanceOfNpc where npcInstanceID=?;"
		<<npcInstanceID
		>>npcID;

		databaseMutex.unlock();
		return npcID;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

bool DatabaseTool::addNpc(
		 	int npcID, 
		 	string description, 
		 	vector<string> keywords,
		 	string longDesc,
		 	string shortDesc
		 	){
	try{
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "INSERT INTO npcs VALUES ( ?, ?, ?, ?, ?);"
		<<npcID
		<<description
		<<concatKeywords(keywords)
		<<longDesc
		<<shortDesc;

		databaseMutex.unlock();
		return true;

	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

bool DatabaseTool::addZone(
		 	int zoneID,
		 	string zoneName,
		 	string description,
		 	vector<ExtendedDescription> extendedDescriptions,
		 	vector<Door> doors
		 	){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db <<FOREIGN_KEY_ON;
		db << "insert into zones (zoneID, zoneName, zoneDescription) values (?,?,?)"
		<<zoneID
		<<zoneName
		<<description;

		for(auto& extendedDesc: extendedDescriptions) {
			db << "insert into zone_ext_descriptions values (NULL, ?, ?, ?)"
			<<zoneID
			<<extendedDesc.description
			<<concatKeywords(extendedDesc.keywords);
		}

		for(auto& door: doors) {
			db << "insert into doors values (NULL, ?, ?, ?, ?, ?)"
			<<zoneID
			<<door.description
			<<concatKeywords(door.keywords)
			<<door.direction
			<<door.goesTo;
		}

		databaseMutex.unlock();
		return true;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() <<std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

string DatabaseTool::getZoneName(int zoneID){
	try {
		databaseMutex.lock();
		string zoneName = "";
		database db( DB_LOCATION );

		db <<"select zoneName from zones where zoneID=?;"
		<<zoneID
		>>zoneName;

		databaseMutex.unlock();
		return zoneName;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}
}

string DatabaseTool::getZoneDesc(int zoneID){
	try {
		databaseMutex.lock();
		string description = "";
		database db( DB_LOCATION );

		db << "select zoneDescription from zones where zoneID=?;"
		<<zoneID
		>>description;

		databaseMutex.unlock();
		return description;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}
}

string DatabaseTool::getZoneExtendedDesc(int zoneID, string keyword){
	try {
		databaseMutex.lock();
		string extendedDesc = "";
		database db( DB_LOCATION );

		db << "select keywords, description from zone_ext_descriptions where zoneID=?;"
		<<zoneID
		>>[&](string keywords, string description) {
			if(keywords.find(keyword) != string::npos) {
				extendedDesc = description;
			}
		};

		databaseMutex.unlock();
		return extendedDesc;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() <<std::endl;
		}

		databaseMutex.unlock();
		return "";
	}

}

int DatabaseTool::getDirectionID(int zoneID, string direction){
	try {
		databaseMutex.lock();
		int linksTo  = 0;
		database db( DB_LOCATION );
		boost::to_lower(direction);

		db << "select keywords, direction, linksTo from doors where zoneID=?;"
		<<zoneID
		>>[&](string keywords, string doorDirection, int linksToID) {
			if((keywords.find(direction) != string::npos) || (direction == doorDirection)) {
				linksTo = linksToID;
			}
		};

		databaseMutex.unlock();
		return linksTo;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

string DatabaseTool::getDirectionDesc(int zoneID, string direction){
	try {
		string doorDescription = "There is nothing in that direction.";
		
		databaseMutex.lock();
		database db( DB_LOCATION );
		boost::to_lower(direction);

		db << "select direction, keywords, description from doors where zoneID=?;"
		<<zoneID
		>>[&](string doorDirection, string keywords, string description) {
			if((keywords.find(direction) != string::npos) || (doorDirection == direction)) {
				doorDescription = description;
			}		
		};

		databaseMutex.unlock();
		return doorDescription;
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}
}

string DatabaseTool::concatKeywords(vector<string> keywords) {
	string combinedKeywords = "";
	for(auto& keyword: keywords) {
		combinedKeywords = combinedKeywords + keyword + " ";
	}
	return combinedKeywords;

}

bool DatabaseTool::addItem(Item item) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;

		db << "insert into items (itemID, shortDescription, description, longDescription, keywords, isPickable, isEquippable, isStackable, isContainer) values (?, ?, ?, ?, ?, ?, ?, ?, ?)"
		<<item.itemID
		<<item.shortDesc
		<<item.description
		<<item.longDesc
		<<concatKeywords(item.keywords)
		<<item.isPickable
		<<item.isEquippable
		<<item.isStackable
		<<item.isContainer;

		databaseMutex.unlock();
		return true;

	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() <<std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

vector<Item> DatabaseTool::getItemsInInventory(int charID) {
	vector<Item> items;
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "select X.itemID, shortDescription, description, longDescription, keywords, isPickable, isEquippable, isStackable, isContainer, quantity, isEquipped from items X, player_inventory Y where X.itemID = Y.itemID and charID =?"
		<<charID
		>>[&](int itemID, string shortDescription, string description, string longDescription, string keywords, int isPickable, int isEquippable, int isStackable, int isContainer, int quantity, int isEquipped) {
			vector<string> keywordsVector;
			boost::split(keywordsVector, keywords, boost::is_any_of(" "));

			Item item(itemID, longDescription, shortDescription, description, keywordsVector);
			item.isPickable = isPickable;
			item.isEquippable = isEquippable;
			item.isStackable = isStackable;
			item.isContainer = isContainer;
			item.quantity = quantity;
			item.isEquipped = isEquipped;
			items.push_back(item);
		};
		databaseMutex.unlock();
		//cout << "[DB] Got items" << endl;

		return items;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return items;
	}
}

vector<string> DatabaseTool::getItemsInZone(int zoneID) {
	vector<string> items;
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "select shortDescription from items X, instanceOfItem Y where X.itemID == Y.itemID and containerID is null and Y.zoneID ==?"
		<<zoneID
		>>[&](string desc) {
			items.push_back(desc);
		};

		databaseMutex.unlock();
		return items;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;;
		}

		databaseMutex.unlock();
		return items;

	}

}


bool DatabaseTool::spawnItemInZone(int itemID, int zoneID){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db << "insert into instanceOfItem values (NULL, ?, ?, NULL)"
		<<itemID
		<<zoneID;

		databaseMutex.unlock();
		return true;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

bool DatabaseTool::spawnItemInNpcInv(int itemID, int npcInstanceID){
	try {
		databaseMutex.lock();
		int canPickUp;
		database db (DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db << "select isPickable from items where itemID = ?;"
		<<itemID
		>>canPickUp;

		if(canPickUp > 0) {
			db << "INSERT INTO npc_inventory VALUES ( NULL, ?, ?, 0, 0);"
			<<npcInstanceID
			<<itemID;

			databaseMutex.unlock();
			return true;
		} else {
			databaseMutex.unlock();
			return false;
		}
	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

bool DatabaseTool::spawnItemInCharacterInv(int itemID, int charID){
	try {
		databaseMutex.lock();
		int canPickUp;
		database db (DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db << "select isPickable from items where itemID = ?;"
		<<itemID
		>>canPickUp;

		if(canPickUp > 0) {
			db << "INSERT INTO player_inventory VALUES ( NULL, ?, ?, 1, 0);"
			<<charID
			<<itemID;

			databaseMutex.unlock();
			return true;
		} else {
			databaseMutex.unlock();
			return false;
		}
	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

bool DatabaseTool::spawnItemInItem(int itemID, int itemInstanceID) {
	try {
		databaseMutex.lock();
		int canPickUp;
		int isContainer;
		int zoneID;
		database db (DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db << "select isPickable from items where itemID = ?;"
		<<itemID
		>>canPickUp;

		db << "select isContainer, Y.zoneID from items X, instanceOfItem Y where X.itemID = Y.itemID and itemInstanceID = ?;"
		<<itemInstanceID
		>> [&](int container, int zone) {
			isContainer = container;
			zoneID = zone;
		};

		if((canPickUp > 0) && (isContainer > 0)) {
			db << "INSERT INTO instanceOfItem VALUES ( NULL, ?, ?, ? );"
			<<itemID
			<<zoneID
			<<itemInstanceID;

			databaseMutex.unlock();
			return true;
		} else {
			databaseMutex.unlock();
			return false;
		}
	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}

//!!!!!!!!!!!!!! depreciated function
// bool DatabaseTool::moveItem(int itemInstanceID, Transfer where, int toID){
// 	try {
// 		int canPickUp;
// 		int isContainer;
// 		database db(DB_LOCATION);
// 		db << "select canPickUp from items X, instanceOfItem Y where X.itemID = Y.itemID and itemInstanceID = ?;"
// 		<<itemInstanceID
// 		>>canPickUp;
// 		switch(where) {
// 			case toCharacter:
// 				if(canPickUp > 0) {
// 					db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, zoneID = NULL, npcInstanceID = NULL, charID = ? WHERE itemInstanceID = ?;"
// 					<<toID
// 					<<itemInstanceID;
// 					return true;
// 				} else {
// 					return false;
// 				}
// 			case toZone:
// 				db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, charID = NULL, npcInstanceID = NULL, zoneID = ? WHERE itemInstanceID = ?;"
// 				<<toID
// 				<<itemInstanceID;
// 				return true;
// 			case toNpc:
// 				if(canPickUp > 0) {
// 					db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, zoneID = NULL, npcInstanceID = NULL, charID = ? WHERE instanceID = ?;"
// 					<<toID
// 					<<itemInstanceID;
// 					return true;
// 				} else {
// 					return false;
// 				}
// 			case toItem:
// 				db << "select isContainer from items X, instanceOfItem Y where X.itemID = Y.itemID and itemInstanceID = ?;"
// 				<< toID
// 				>> isContainer;
// 				if((canPickUp > 0) && (isContainer >0)) {
// 					db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, charID = NULL, zoneID = NULL, npcInstanceID = NULL, otherItemInstanceID = ? WHERE itemInstanceID = ?;"
// 					<<toID
// 					<<itemInstanceID;
// 					return true;
// 				} else {
// 					return false;
// 				}
// 			default:
// 				return false;
// 		}
// 	} catch(sqlite_exception e) {
// 		return false;
// 	}

// }

// bool DatabaseTool::deleteItem(int itemInstanceID){
// 	string statment = "delete from instanceOfItem where itemInstanceID=" + to_string(itemInstanceID) + ";";
// 	return executeSQLInsert(statment);
// }

bool DatabaseTool::addResetCommand(ResetCommand command){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db << "insert into resetCommands values (NULL, ?, ?, ?, ?, ?, ?, ?);"
		<<command.action
		<<command.id
		<<command.slot
		<<command.npcLimit
		<<command.room
		<<command.state
		<<command.container;

		databaseMutex.unlock();
		return true;

	}catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

Attributes DatabaseTool::getAttributes(int id, Target characterOrNpc){
	Attributes attributes;
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		if(characterOrNpc == Target::character) {
			db << "select charID, level, experience, requiredExp, health, maxHealth, mana, maxMana, strength, intelligence, dexterity, charisma, ringSlot, headSlot, chestSlot, greavesSlot, feetSlot, handSlot, weponSlot from playerAttributes where charID = ?;"
			<< id
			>>[&](int charID, int level, int experience, int requiredExp, int health, int maxHealth, int mana, int maxMana, int strength, int intelligence, int dexterity, int charisma, int ringSlot, int headSlot, int chestSlot, int greavesSlot, int feetSlot, int handSlot, int weponSlot) {
				Attributes characterAttributes(charID, level, experience, requiredExp, health, maxHealth, mana, maxMana, strength, intelligence, dexterity, charisma, ringSlot, headSlot,  chestSlot, greavesSlot, feetSlot, handSlot, weponSlot);
				attributes = characterAttributes;
			};
		} else if(characterOrNpc == Target::npc) {
			db << "select npcInstanceID, level, experience, requiredExp, health, maxHealth, mana, maxMana, strength, intelligence, dexterity, charisma, ringSlot, headSlot, chestSlot, greavesSlot, feetSlot, handSlot, weponSlot from npcAttributes where npcInstanceID = ?;"
			<< id
			>>[&](int npcInstanceID, int level, int experience, int requiredExp, int health, int maxHealth, int mana, int maxMana, int strength, int intelligence, int dexterity, int charisma, int ringSlot, int headSlot, int chestSlot, int greavesSlot, int feetSlot, int handSlot, int weponSlot) {
				Attributes npcAttributes(npcInstanceID, level, experience, requiredExp, health, maxHealth, mana, maxMana, strength, intelligence, dexterity, charisma, ringSlot, headSlot,  chestSlot, greavesSlot, feetSlot, handSlot, weponSlot);
				attributes = npcAttributes;
			};
		}

		databaseMutex.unlock();
		return attributes;

	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return attributes;
	}
}

bool DatabaseTool::updateAttributes(Attributes attributes, Target characterOrNpc){
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		string targetTable = "";
		string idColName = "";

		if(characterOrNpc == Target::character) {
			targetTable = "playerAttributes";
			idColName = "charID";
		} else if(characterOrNpc == Target::npc) {
			targetTable = "npcAttributes";
			idColName = "npcInstanceID";
		}
		db << "begin;";
		
		if(attributes.level != 0) {
			int level = 0;
			string selectStatment = "select level from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>level;

			level = level + attributes.level;

			string updateStatement = "update " + targetTable + " set level = ? where " + idColName + " = ?;";
			db << updateStatement
			<<level
			<<attributes.id;

		}
		if(attributes.experience != 0) {
			int experience = 0;
			string selectStatment = "select experience from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>experience;

			experience = experience + attributes.experience;

			string updateStatement = "update " + targetTable + " set experience = ? where " + idColName + " = ?;";
			db << updateStatement
			<<experience
			<<attributes.id;

		}
		if(attributes.requiredExperience != 0) {
			int requiredExperience = 0;
			string selectStatment = "select requiredExp from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>requiredExperience;

			requiredExperience = requiredExperience + attributes.requiredExperience;

			string updateStatement = "update " + targetTable + " set requiredExp = ? where " + idColName + " = ?;";
			db << updateStatement
			<<requiredExperience
			<<attributes.id;
		}
		if(attributes.health != 0) {
			int health = 0;
			string selectStatment = "select health from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>health;

			health = health + attributes.health;

			// Cap health at max health
			int maxHealth = 0;
			string selectMaxStatment = "select maxHealth from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectMaxStatment
			<<attributes.id
			>>maxHealth;

			if ((attributes.health >= maxHealth) || (health >= maxHealth)) {
				health = maxHealth;
			}

			string updateStatement = "update " + targetTable + " set health = ? where " + idColName + " = ?;";
			db << updateStatement
			<<health
			<<attributes.id;
		}
		if(attributes.maxHealth != 0) {
			int maxHealth = 0;
			string selectStatment = "select maxHealth from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>maxHealth;

			maxHealth = maxHealth + attributes.maxHealth;

			string updateStatement = "update " + targetTable + " set maxHealth = ? where " + idColName + " = ?;";
			db << updateStatement
			<<maxHealth
			<<attributes.id;
		}
		if(attributes.mana != 0) {
			int mana = 0;
			string selectStatment = "select mana from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>mana;

			mana = mana + attributes.mana;

			// Cap mana at max mana

			int maxMana = 0;
			string selectMaxStatment = "select maxMana from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectMaxStatment
			<<attributes.id
			>>maxMana;

			if ((attributes.mana >= maxMana) || (mana >= maxMana)) {
				mana = maxMana;
			}

			string updateStatement = "update " + targetTable + " set mana = ? where " + idColName + " = ?;";
			db << updateStatement
			<<mana
			<<attributes.id;
		}
		if(attributes.maxMana != 0) {
			int maxMana = 0;
			string selectStatment = "select maxMana from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>maxMana;

			maxMana = maxMana + attributes.maxMana;

			string updateStatement = "update " + targetTable + " set maxMana = ? where " + idColName + " = ?;";
			db << updateStatement
			<<maxMana
			<<attributes.id;
		}
		if(attributes.strength != 0) {
			int strength = 0;
			string selectStatment = "select strength from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>strength;

			strength = strength + attributes.strength;

			string updateStatement = "update " + targetTable + " set strength = ? where " + idColName + " = ?;";
			db << updateStatement
			<<strength
			<<attributes.id;
		}
		if(attributes.intelligence != 0) {
			int intelligence = 0;
			string selectStatment = "select intelligence from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>intelligence;

			intelligence = intelligence + attributes.intelligence;

			string updateStatement = "update " + targetTable + " set intelligence = ? where " + idColName + " = ?;";
			db << updateStatement
			<<intelligence
			<<attributes.id;
		}
		if(attributes.dexterity != 0) {
			int dexterity = 0;
			string selectStatment = "select dexterity from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>dexterity;

			dexterity = dexterity + attributes.dexterity;

			string updateStatement = "update " + targetTable + " set dexterity = ? where " + idColName + " = ?;";
			db << updateStatement
			<<dexterity
			<<attributes.id;
		}
		if(attributes.charisma != 0) {
			int charisma = 0;
			string selectStatment = "select charisma from " + targetTable + " where " + idColName + " = ?;"; 
			db << selectStatment
			<<attributes.id
			>>charisma;

			charisma = charisma + attributes.charisma;

			string updateStatement = "update " + targetTable + " set charisma = ? where " + idColName + " = ?;";
			db << updateStatement
			<<charisma
			<<attributes.id;
		}

		db << "commit;";

		databaseMutex.unlock();
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}

}

bool DatabaseTool::equipItem(int charID, string item) {
	bool foundItem = false;
	try{
		databaseMutex.lock();
		database db (DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db << "select shortDescription, keywords, ownershipID, isEquippable from items X, player_inventory Y where X.itemID = Y.itemID and charID = ?;"
		<<charID
		>>[&](string shortdesc, string keywords, int itemInstanceID, int equippableSlot) {
			if((shortdesc.find(item) != string::npos) || (keywords.find(item) != string::npos)) {
				int currentlyEquiped;
				db << "select " + getSlot(equippableSlot) + " from playerAttributes where charID = ?;"
				<<charID
				>>currentlyEquiped;

				if(currentlyEquiped != 0) {
					db <<"update player_inventory set isEquipped = 0 where ownershipID = ?;"
					<< currentlyEquiped;
				}
				db <<"update player_inventory set isEquipped = 1 where ownershipID = ?;"
				<<itemInstanceID;
				
				string statment = "update playerAttributes set " + getSlot(equippableSlot) + "= ?  where charID = ? ;";
				db << statment
				<<itemInstanceID
				<<charID;
				foundItem = true;
			}
		};

		databaseMutex.unlock();
		return foundItem;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}

}

bool DatabaseTool::pickUp(int charID, string item) {
	int foundItemID;
	int foundInstanceID;
	int canPickUp;
	bool equipped = false;
	try{
		databaseMutex.lock();
		database db (DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db << "select shortDescription, keywords, X.itemID, isPickable, itemInstanceID from items X, instanceOfItem Y, characters Z where X.itemID = Y.itemID and Z.location = Y.zoneID and containerID is null and Z.charID = ?;"
		<<charID
		>>[&](string shortdesc, string keywords, int itemID, int isPickable, int instanceID) {
			if((shortdesc.find(item) != string::npos) || (keywords.find(item) != string::npos)) {
				canPickUp = isPickable;
				foundItemID = itemID;
				foundInstanceID = instanceID;
			}
		};
		if(canPickUp) {
			db << "insert into player_inventory values (NULL, ?, ?, 1, 0)"
			<<charID
			<<foundItemID;
			db << "delete from instanceOfItem where itemInstanceID = ?"
			<<foundInstanceID;
			equipped = true;
		}

		databaseMutex.unlock();
		return equipped;

	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}


bool DatabaseTool::dropItem(int charID, string item) {
	int foundItemID;
	int foundInvID;
	int zoneID;
	try{
		database db (DB_LOCATION);
		db << "SELECT itm.shortDescription, itm.keywords, itm.itemID, inv.ownershipID FROM items itm, player_inventory inv, characters chr WHERE itm.itemID = inv.itemID AND chr.charID = ?;"
		<<charID
		>>[&](string shortdesc, string keywords, int itemID, int ownershipID) {
			if((shortdesc.find(item) != string::npos) || (keywords.find(item) != string::npos)) {
				foundItemID = itemID;
				foundInvID = ownershipID;
			}
		};

		zoneID = DatabaseTool::getCharsLocation( charID );
		
		db 	<< "INSERT INTO instanceOfItem (itemID,zoneID) VALUES (?, ?)"
			<< foundItemID
			<< zoneID;
		
		db 	<< "DELETE FROM player_inventory WHERE ownershipID = ?"
			<< foundInvID;
		
		return true;
	} catch(sqlite_exception e) {
		cout << e.what() << endl;
		return false;
	}
}


string DatabaseTool::getSlot(int equiableTo) {
	switch(equiableTo){
		case 1:
			return "ringSlot";
		case 2:
			return "headSlot";
		case 3:
			return "chestSlot";
		case 4:
			return "greavesSlot";
		case 5:
			return "feetSlot";
		case 6:
			return "handSlot";
		case 7:
			return "weponSlot";
		default:
			return "";
	}

}

bool DatabaseTool::setCombatFlag(int id, bool inCombat, Target characterOrNpc) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);
		bool isSet = false;

		if(characterOrNpc == Target::character) {
			if(inCombat) {
				db << "update charactersOnline set inCombat = 1 where charID =?"
				<<id;
			} else {
				db << "update charactersOnline set inCombat = 0 where charID =?"
				<<id;					
			}
			isSet = true;
		} else if(characterOrNpc == Target::npc) {
			if(inCombat) {
				db << "update instanceOfNpc set inCombat = 1 where npcInstanceID = ?"
				<< id;
			} else {
				db << "update instanceOfNpc set inCombat = 0 where npcInstanceID = ?"
				<< id;	
			}
			isSet = true;
		}

		databaseMutex.unlock();
		return isSet;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}

}

bool DatabaseTool::inCombat(int id, Target characterOrNpc) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);
		int inCombat;

		if(characterOrNpc == Target::character) {
			db << "select inCombat from charactersOnline where charID = ?"
			<<id
			>>inCombat;
		} else if(characterOrNpc == Target::npc) {
			db << "select inCombat from instanceOfNpc where npcInstanceID = ?"
			<<id
			>>inCombat;
		}

		databaseMutex.unlock();
		return inCombat;
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

int DatabaseTool::getNpcInstanceIDFromName(string name, int zoneID) {
	try {
		databaseMutex.lock();
		int instanceID = 0;
		database db(DB_LOCATION);

		db << "select npcInstanceID, keywords, shortdesc from instanceOfNpc X, npcs Y where X.npcID = Y.npcID and zoneID = ?"
		<<zoneID
		>>[&](int npcInstanceID, string keywords, string shortDesc) {
			if((keywords.find(name) != string::npos) || (shortDesc.find(name) != string::npos)) {
				instanceID = npcInstanceID;
			}
		};

		databaseMutex.unlock();
		return instanceID;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}

}

string DatabaseTool::look(int charID, string word) {
	int zoneID = getCharsLocation(charID);
	if(word == "") {
		string description = getZoneDesc(zoneID);
		return description;
	} else if(word == "inventory") {
		vector<Item> itemsInIneventory = getItemsInInventory(charID);
		string items = "In your inventory you have: ";
		for(auto& item: itemsInIneventory) {
			items = items + item.shortDesc + ", ";
		}

		if(items == "In your inventory you have: ") {
			items = "You have no items in your inventory.  ";
		}

		return items.substr(0, items.size()-2);

	} else  if(word == "objects") {
		vector<string> itemsInZone = getItemsInZone(zoneID);
		string items = "You see ";
		for(auto& item: itemsInZone) {
			items = items + item + ", ";
		}

		if(items == "You see ") {
			items = "You see no objects.  ";
		}

		return items.substr(0, items.size()-2);

	} else if(word == "players") {
		string players = "You see ";
		database db(DB_LOCATION);
		db<<"select name from characters C, charactersOnline O where C.charID == O.charID and C.charID <> ? and C.location = ?"
		<<charID
		<<zoneID
		>>[&](string name) {
			players = players + name + ", ";
		};

		if(players == "You see ") {
			players = "You see no players.  ";
		}
		return players.substr(0, players.size()-2);

	} else if(word == "people") {
		string npcs = "You see ";
		database db(DB_LOCATION);
		db<<"select shortDesc from npcs N, instanceOfNpc I where N.npcID == I.npcID and I.zoneID = ?"
		<<zoneID
		>>[&](string npc) {
			npcs = npcs + npc + ", ";
		};

		if(npcs == "You see ") {
			npcs = "You see no people.  ";
		}

		return npcs.substr(0, npcs.size()-2);
	}

	string description = "";

	description = getZoneExtendedDesc(zoneID, word);
	if(!description.empty()) {
		return description;
	}


	description = getDirectionDesc(zoneID, word);
	if(!description.empty()) {
		return description;
	}


	description = findPlayerDescription(charID, zoneID, word);
	if(!description.empty()) {
		return description;
	}

	description = findNpcDescription(zoneID, word);
	if(!description.empty()) {
		return description;
	}

	description = findItemDescription(charID, zoneID, word);
	if(!description.empty()) {
		return description;
	}

	return "You see nothing related to " + word;

}

string DatabaseTool::findNpcDescription(int zoneID, string word) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);
		string foundDescription = "";

		db << "select description, longDesc, keywords, shortDesc from npcs N, instanceOfNpc I where N.npcID == I.npcID and zoneID == ?;"
		<<zoneID
		>>[&] (string description, string longDescription, string keywords, string shortDesc) {

			if(shortDesc.find(word) != string::npos) {
				foundDescription = longDescription;
			}else if(keywords.find(word) != string::npos) {
				foundDescription = description;
			}
		};

		databaseMutex.unlock();
		return foundDescription;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}

}

string DatabaseTool::findPlayerDescription(int lookerID, int zoneID, string name) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		string description;
		db << "select description from charactersOnline O, characters C where C.name == ? and C.charID == O.charID and C.location = ?;"
		<<name
		<<zoneID
		>>description;

		databaseMutex.unlock();
		return description;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}

}

string DatabaseTool::findItemDescription(int charID, int zoneID, string word) {
	try {
		database db(DB_LOCATION);
		string foundDescription = "";

		db << "select description, shortDescription, keywords from items I, player_inventory P where I.itemID == P.itemID and P.charID == ?"
		<<charID
		>>[&] (string description, string shortDesc, string keywords) {
			if(keywords.find(word) != string::npos) {
				if(description == "") {
					foundDescription = shortDesc;
				} else {
					foundDescription = description;
				}
			}
		};

		if(foundDescription == "") {
			db << "select longDescription, keywords, shortDescription from items X, instanceOfItem Y where X.itemID == Y.itemID and zoneID == ?"
			<<zoneID
			>>[&](string longDesc, string keywords, string shortDesc) {
				if((keywords.find(word) != string::npos) || (shortDesc.find(word) != string::npos)) {
					foundDescription = longDesc;
				}
			};
		}

		databaseMutex.unlock();
		return foundDescription;

	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "";
	}
}

void DatabaseTool::executeCommands() {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";
		
		db<< "select action, id, room from resetCommands where action == ? or action == ?"
		<<"npc"
		<<"object"
		>>[&](string action, int id, int room) {

			try {
				if(action == "npc") {
					db << "insert into instanceOfNpc values (NULL, ?, ?, 1, 0)"
					<<id
					<<room;
					int npcInstanceID = db.last_insert_rowid();
					db << "INSERT INTO npcAttributes VALUES (?, 1, 0, 100, 100, 100, 100, 100, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0);"
					<<npcInstanceID;

				} else if(action == "object") {
					db << "insert into instanceOfItem values (NULL, ?, ?,NULL)"
					<<id
					<<room;

				}
			} catch (sqlite_exception e) {

			}

		};

		databaseMutex.unlock();
	} catch(sqlite_exception e){
		if(verbosity > 0) {
			cout << e.what();
		}

		databaseMutex.unlock();

	}
}


bool DatabaseTool::unEquip(int charID, string item) {
	bool foundItem = false;
	try{
		databaseMutex.lock();
		database db (DB_LOCATION);

		db << "select shortDescription, keywords, ownershipID, isEquippable from items X, player_inventory Y where X.itemID = Y.itemID and charID = ?;"
		<<charID
		>>[&](string shortdesc, string keywords, int itemInstanceID, int equippableSlot) {
			if((shortdesc.find(item) != string::npos) || (keywords.find(item) != string::npos)) {
				db <<"update player_inventory set isEquipped = 0 where ownershipID = ?;"
				<<itemInstanceID;
				
				string statment = "update playerAttributes set " + getSlot(equippableSlot) + "= 0  where charID = ? ;";
				db << statment
				<<charID;
				foundItem = true;
			}
		};

		databaseMutex.unlock();
		return foundItem;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}	
}





// // NEW STUFF by pavel

int DatabaseTool::createNewZone( string zoneName, string zoneDesc ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;

		db 	<< "INSERT INTO zones (zoneName,zoneDescription) VALUES (?,?);"
			<< zoneName
			<< zoneDesc;

		databaseMutex.unlock();
		return db.last_insert_rowid();
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}


int DatabaseTool::createNewZone( int zoneID, string zoneName, string zoneDesc ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;

		db 	<< "INSERT INTO zones (zoneID,zoneName,zoneDescription) VALUES (?,?,?);"
			<< zoneID
			<< zoneName
			<< zoneDesc;

		databaseMutex.unlock();
		return db.last_insert_rowid();
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}


void DatabaseTool::deleteZone( int zoneID ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;

		db 	<< "DELETE FROM zones WHERE zoneID = ?;"
			<< zoneID;

		databaseMutex.unlock();

	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return;
	}
}


bool DatabaseTool::addExtendedDescriptionToZone( int zoneID, string desc, string keywords ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;

		db 	<< "INSERT INTO zone_ext_descriptions (zoneID,description,keywords) VALUES (?,?,?);"
			<< zoneID
			<< desc
			<< keywords;

		databaseMutex.unlock();
		return true;
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}


int DatabaseTool::addDoorToZone( int zoneID, string description, string direction, int pointer, string keywords ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;

		db 	<< "INSERT INTO doors (zoneID,description,keywords,direction,linksTo) VALUES (?,?,?,?,?);"
			<< zoneID
			<< description
			<< keywords
			<< direction
			<< pointer;

		databaseMutex.unlock();
		return db.last_insert_rowid();
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}


void DatabaseTool::deleteDoor( int doorID ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << FOREIGN_KEY_ON;

		db 	<< "DELETE FROM doors WHERE doorID = ?;"
			<< doorID;

		databaseMutex.unlock();

	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return;
	}
}


string DatabaseTool::getDoorDescriptionAt( int zoneID, string direction ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		string fetchedDesc;
		db 	<< "SELECT description FROM doors WHERE zoneID == ? AND direction == ?;"
			<< zoneID
			<< direction
			>> fetchedDesc;

		databaseMutex.unlock();
		return fetchedDesc;
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return "There is nothing there.";
	}
}


int DatabaseTool::getZoneIDBehindDoorAt( int zoneID, string direction ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		int fetchedZoneID;
		db 	<< "SELECT linksTo FROM doors WHERE zoneID == ? AND direction == ?;"
			<< zoneID
			<< direction
			>> fetchedZoneID;


		databaseMutex.unlock();
		return fetchedZoneID;
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}


bool DatabaseTool::moveCharacterToZone( int charID, int zoneID ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db 	<< "UPDATE characters SET location = ? WHERE charID == ?;"
			<< zoneID
			<< charID;

		databaseMutex.unlock();
		return true;
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}


void DatabaseTool::deleteObject( int objectID ) {
	try {
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";

		db 	<< "DELETE FROM items WHERE itemID = ?;"
			<< objectID;

	} catch ( exception& e ) {
		return;
	}
}


int DatabaseTool::createNewItem( string shrtDesc, string desc, string lngDesc, string keywords ) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);
		db << FOREIGN_KEY_ON;

		db 	<< "INSERT INTO items (shortDescription,description,longDescription,keywords,isPickable,isEquippable,isStackable,isContainer) VALUES (?,?,?,?,1,0,0,0);"
			<< shrtDesc
			<< desc
			<< lngDesc
			<< keywords;
		
		databaseMutex.unlock();
		return db.last_insert_rowid();
	} catch ( exception& e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return 0;
	}
}


bool DatabaseTool::signUserIn( string userName, string password ){
	try {

		databaseMutex.lock();
		database db( DB_LOCATION );
		
		int loggedIn = 0;
		db	<< "SELECT EXISTS(SELECT 1 FROM users WHERE userName == ? AND password == ? AND signedOn == 0 LIMIT 1);"
			<< userName
			<< password
			>> loggedIn;
		
		db 	<<	"UPDATE users SET signedOn = 1 WHERE userName == ? AND password == ? AND signedOn == 0;"
			<<	userName
			<<	password;
		
		databaseMutex.unlock();
		return loggedIn;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
			std::cout << "[Database] Failed to open file.";
		}

		databaseMutex.unlock();
		return false;
	}
}


bool DatabaseTool::signUserOut( int userID ){
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );
		
		int loggedOut = 0;
		db	<< "SELECT EXISTS(SELECT 1 FROM users WHERE userID == ? AND signedOn == 1 LIMIT 1);"
			<< userID
			>> loggedOut;
		
		db 	<<	"UPDATE users SET signedOn = 0 WHERE userID == ? AND signedOn == 1;"
			<<	userID;
		
		databaseMutex.unlock();
		return loggedOut;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}


void DatabaseTool::clearAllSessions() {
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );
	
		db << "DELETE FROM charactersOnline;";

		databaseMutex.unlock();
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
	}
}


void DatabaseTool::signOffAllUsers() {
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );
	
		db << "UPDATE users SET signedOn = 0";

		databaseMutex.unlock();
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		databaseMutex.unlock();
	}
}


bool DatabaseTool::testValidity() {
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );

		databaseMutex.unlock();
		return true;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}
}

bool DatabaseTool::setAllNotInCombat() {
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );

		db << "update charactersOnline set inCombat = 0;";
		db << "update instanceOfNpc set inCombat = 0;";

		databaseMutex.unlock();
		return true;
	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}

		databaseMutex.unlock();
		return false;
	}

}

int DatabaseTool::checkCommand(string command) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		int header;
		db << "select header from commands where command = ?;"
		<<command
		>> header;

		databaseMutex.unlock();
		return header;
	} catch(sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		databaseMutex.unlock();
		return INVALID_COMMAND;
	}
}

bool DatabaseTool::knowsSpell(int charID, string spellName) {
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);


		db << "select knownID from knownSpells where charID = ? and spellName = ?"
		<< charID
		<<spellName;

		databaseMutex.unlock();
		return true;

	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		databaseMutex.unlock();
		return false;
	}
}

Spell DatabaseTool::getSpell(string spellName) {
	Spell spell;
	try {
		databaseMutex.lock();
		database db(DB_LOCATION);

		db << "select * from spells where spellName = ?"
		<<spellName
		>>[&](string spellName, int minLevel, int cost, int archetypeID, string effect, string hitChar, string hitRoom, string hitVict) {
			Spell spellToGet(spellName, minLevel, cost, archetypeID, effect, hitChar, hitRoom, hitVict);
			spell = spellToGet;
		};

		databaseMutex.unlock();
		std::cout << " MIN LEVEL FOR SPELL : " << spell.minLevel << std::endl;
		return spell;

	} catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		databaseMutex.unlock();
		return spell;
	}
}

vector< string > DatabaseTool::getAllNPCsInZone( int zoneID ) {
	vector< string > npcs;
	
	try {
		database db( DB_LOCATION );
		
		
		db	<< "SELECT shortDesc FROM npcs N, instanceOfNpc I WHERE N.npcID == I.npcID AND I.zoneID = ?"
			<< zoneID
			>> [ & ]( string npc ) {
				npcs.push_back( npc );
			};
		
		return npcs;
	} catch( sqlite_exception e ) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		return npcs;
	}
}


vector< string > DatabaseTool::getAllPlayersInZone( int charID, int zoneID ) {
	vector< string > players;

	try {
		database db( DB_LOCATION );


		db	<< "SELECT name FROM characters C, charactersOnline O WHERE C.charID == O.charID AND C.charID <> ? AND C.location = ?"
			<< charID
			<< zoneID
			>> [ & ]( string player ) {
				players.push_back( player );
			};

		return players;
	} catch( sqlite_exception e ) {
		std::cerr << e.what() << std::endl;
		return players;
	}
}


bool DatabaseTool::userExists( string userName ) {
	try {
		int userID = 0;
		
		databaseMutex.lock();
		database db( DB_LOCATION );
		
		db	<< "SELECT userID FROM users WHERE userName = ?;"
			<< userName
			>> userID;
		databaseMutex.unlock();
		
		return userID;
		
	} catch ( sqlite_exception e ) {
		databaseMutex.unlock();
		
		std::cerr << e.what() << std::endl;
		return false;
	}
}


vector< int > DatabaseTool::getAllUserCharIDs( int userID ) {
	vector< int > charIDs;
	
	try {
		databaseMutex.lock();
		database db( DB_LOCATION );
		
		db	<< "SELECT charID FROM characters WHERE userID = ?"
			<< userID
			>> [ & ]( int charID ) {
				charIDs.push_back( charID );
			};
		
		databaseMutex.unlock();
		return charIDs;
	} catch ( sqlite_exception e ) {
		std::cerr << e.what() << std::endl;
		databaseMutex.unlock();
		return charIDs;
	}
}


string DatabaseTool::getCharDescription( int charID ) {
	try {
		string desc;
		
		databaseMutex.lock();
		database db( DB_LOCATION );

		db	<< "SELECT description FROM characters WHERE charID = ?"
			<< charID
			>> desc;

		databaseMutex.unlock();
		return desc;
	} catch ( sqlite_exception e ) {
		std::cerr << e.what() << std::endl;
		databaseMutex.unlock();
		return "";
	}
}

int DatabaseTool::getNpcLocation(int npcInstanceID) {
	try {
		int zoneID = 0;

		databaseMutex.lock();
		database db( DB_LOCATION);

		db << "SELECT zoneID from instanceOfNpc where npcInstanceID = ?"
		<< npcInstanceID
		>> zoneID;

		return zoneID;
		databaseMutex.unlock();

	}catch (sqlite_exception e) {
		if(verbosity > 0) {
			std::cerr << e.what() << std::endl;
		}
		databaseMutex.unlock();
		return 0;
	}
}







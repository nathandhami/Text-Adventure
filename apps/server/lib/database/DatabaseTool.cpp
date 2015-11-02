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

const string DB_LOCATION = "apps/server/databases/adventureDB";
const string INITIAL_ZONE = "3054";
const int PLAYER_OFFLINE = 0;
const int PLAYER_ONLINE = 1;

using namespace std;
using namespace  sqlite;




string DatabaseTool::quotesql( const string& s ) {
	if(s.empty()) {
		return string("NULL");
	} else {
    	return string("'") + s + string("'");
	}
}

bool DatabaseTool::executeSQLInsert(string statment){
	try {
		database db( DB_LOCATION );
		db << "PRAGMA foreign_keys = ON;";
		db << statment;
	return true;
	} catch (sqlite_exception e) {
		return false;
	}


}

bool DatabaseTool::addUser(string userName, string password) {
	string sqlStatment = "INSERT INTO users VALUES ( NULL, " + quotesql(userName) + "," + quotesql(password) + ", 0);";
	return executeSQLInsert(sqlStatment);
}

int DatabaseTool::getUserID(string userName, string password){
	try {
		database db( DB_LOCATION );
		int charID;
		db << "select userID from users where userName= ? AND password= ?;"
		<<userName
		<<password
		>>charID;
		return charID;
	} catch(sqlite_exception e) {
		return 0;
	}
}

int DatabaseTool::getUserAuthencationLevel(int userID) {
	try {
		database db( DB_LOCATION );
		int authencationLevel;
		db << "select authencationLevel from users where userID=?"
		<<userID
		>>authencationLevel;
		return authencationLevel;
	} catch(sqlite_exception e) {
		return 0;
	}
}

bool DatabaseTool::setUserAuthencationLevel(int userID, int authencationLevel) {
	string statment = "UPDATE users SET authencationLevel = " + to_string(authencationLevel) + " WHERE userID = " + to_string(userID) + ";";
	return executeSQLInsert(statment);
}

string DatabaseTool::getPassword(int userID) {
	try {
		string password = "";
		database db( DB_LOCATION );
		db << "select password from users where userID=?;"
		<<userID
		>>password;
		return password;
	} catch(sqlite_exception e) {
		return "";
	}

}

bool DatabaseTool::addCharacter(string name, int userID, string description){
	try {
		database db(DB_LOCATION);
		db << "PRAGMA foreign_keys = ON;";
		db << "INSERT INTO characters VALUES ( NULL, ? , ?, ?);"
		<<name
		<<userID
		<<INITIAL_ZONE;
		int charID = db.last_insert_rowid();

		db << "INSERT INTO playerAttributes VALUES (?, ?, 1, 0, 100, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0);"
		<<charID
		<<description;
		return true;
	} catch(sqlite_exception e) {
		return false;
	}
}

vector<string> DatabaseTool::getCharactersNames(int userID){
	vector<string> names;
	try {
		database db( DB_LOCATION );
		db << "select name from characters where userID=?;"
		<<userID
		>>[&](string name) {
			names.push_back(name);
		};
		return names;
	} catch (sqlite_exception e) {
		return names;
	}
}
		
int DatabaseTool::getCharIDFromName(string name){
	try {
		int charID = 0;
		database db( DB_LOCATION );
		db << "select charID from characters where name=?"
		<<name
		>>charID;
		return charID;
	} catch(sqlite_exception e) {
		return 0;
	}
}

int DatabaseTool::getCharIDInZoneFromName(string name, int zoneID) {
	try {
		int charID = 0;
		database db( DB_LOCATION );
		db << "select X.charID from characters X, charactersOnline Y where X.charID = Y.charID and name=? and location = ?"
		<<name
		<<zoneID
		>>charID;
		return charID;
	} catch(sqlite_exception e) {
		return 0;
	}
}

string DatabaseTool::getCharNameFromID(int charID) {
	try {
		string name = "";
		database db(DB_LOCATION);
		db << "select name from characters where charID = ?;"
		<< charID
		>>name;
		return name;
	} catch (sqlite_exception e) {
		return "";
	}

}

bool DatabaseTool::isCharOnline(int charID){
	try {
		int onlineStatus = 0;
		database db( DB_LOCATION );
		db << "SELECT EXISTS(SELECT 1 FROM charactersOnline WHERE charID= ? LIMIT 1);"
		<<charID
		>>onlineStatus;
		return onlineStatus;
	} catch(sqlite_exception e) {
		return 0;
	}
}

void DatabaseTool::setCharOnline(int charID, string sessionID){
	string sqlStatment = "INSERT INTO charactersOnline VALUES ( " + to_string(charID) + " , " + quotesql(sessionID) + " , 0);";
	executeSQLInsert(sqlStatment);
}

void DatabaseTool::setCharOffline(int charID){
	string sqlStatment = "DELETE FROM charactersOnline WHERE charID = " + to_string(charID) + ";";
	executeSQLInsert(sqlStatment);
}

string DatabaseTool::getSessionID(int charID) {
	try {
		string sessionID;
		database db( DB_LOCATION );
		db << "select sessionID from charactersOnline where charID=?;"
		<<charID
		>>sessionID;
		return sessionID;
	}catch (sqlite_exception e) {
		return "";
	}

}

int DatabaseTool::getCharID(int userID){
	try {
		int charID = 0;
		database db( DB_LOCATION );
		db << "select charID from characters where userID=?;"
		<<userID
		>>charID;
		return charID;
	} catch(sqlite_exception e) {
		return 0;
	}
}

void DatabaseTool::putCharInZone(int charID, int zoneID){
	string sqlStatment = "UPDATE characters SET location = " + to_string(zoneID) + " WHERE charID = " + to_string(charID) + ";";
	executeSQLInsert(sqlStatment);
}

int DatabaseTool::getCharsLocation(int charID){
	try {
		int zoneID = 0;
		database db( DB_LOCATION );
		db<< "select location from characters where charID=?;"
		<<charID
		>>zoneID;
		return zoneID;
	} catch(sqlite_exception e) {
		return 0;
	}
}

vector<int> DatabaseTool::getAllOnlineCharsInZone(int zoneID){
	vector<int> charsInZone;
	database db( DB_LOCATION );
	db << "select x.charID from characters C, charactersOnline X where location= ? AND x.charID = c.charID;"
	<< zoneID
	>>[&](int charID) {
		charsInZone.push_back(charID);
	};
	return charsInZone;
}

bool DatabaseTool::createNpcInstance(int npcID, int zoneID){
	try {
		database db(DB_LOCATION);
		db << "PRAGMA foreign_keys = ON;";
		db << "INSERT INTO instanceOfNpc VALUES ( NULL, ?, ?, 1 , 0);"
		<<npcID
		<<zoneID;

		int npcInstanceID = db.last_insert_rowid();

		db << "INSERT INTO npcAttributes VALUES (?, 1, 0, 100, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0);"
		<<npcInstanceID;
		return true;
	} catch(sqlite_exception e) {
		return false;
	}
}

vector<int> DatabaseTool::getAllAliveNpcsInZone(int zoneID){
	vector<int> npcsInZone;
	database db( DB_LOCATION );
	db << "select npcInstanceID from instanceOfNpc where zoneID=? and isAlive = 1;"
	<< zoneID
	>>[&](int npcID) {
		npcsInZone.push_back(npcID);
	};
	return npcsInZone;
}

void DatabaseTool::deleteNpcInstance(int npcInstanceID){
	string sqlStatment = "delete from instanceOfNpc where npcID=" + to_string(npcInstanceID) + ";";
	executeSQLInsert(sqlStatment);
}

bool DatabaseTool::isNpcAlive(int npcInstanceID){
	try {
		int isAlive;
		database db(DB_LOCATION);
		db << "select isAlive from instanceOfNpc where npcInstanceID = ?;"
		<<npcInstanceID
		>> isAlive;
		return isAlive;
	} catch(sqlite_exception e) {
		return false;
	}

}

void DatabaseTool::respawnAll(){
	string statment = "UPDATE instanceOfNpc SET isAlive = 1;";
	executeSQLInsert(statment);

}

bool DatabaseTool::murderNpc(int npcInstanceID){
	string statment = "UPDATE instanceOfNpc SET isAlive = 0 where npcInstanceID = " + to_string(npcInstanceID) + ";";
	return executeSQLInsert(statment);
}

bool DatabaseTool::reviveNpc(int npcInstanceID){
	string statment = "UPDATE instanceOfNpc SET isAlive = 1 where npcInstanceID = " + to_string(npcInstanceID) + ";";
	return executeSQLInsert(statment);
}

string DatabaseTool::getNpcDesc(int npcInstanceID){
	try {
		string description;
		database db( DB_LOCATION );
		db << "select description from npcs X, instanceOfNpc Y where X.npcID = Y.npcID and npcInstanceID=?;"
		<<npcInstanceID
		>>description;
		return description;
	} catch(sqlite_exception e) {
		return "";
	}
}

string DatabaseTool::getNpcName(int npcInstanceID){
	try {
		string name;
		database db( DB_LOCATION );
		db << "select shortDesc from npcs X, instanceOfNpc Y where X.npcID = Y.npcID and npcInstanceID=?;"
		<<npcInstanceID
		>>name;
		return name;
	} catch(sqlite_exception e) {
		return "";
	}
}

int DatabaseTool::getNpcIDFromInstanceID(int npcInstanceID) {
	try {
		int npcID;
		database db( DB_LOCATION );
		db << "select npcID from instanceOfNpc where npcInstanceID=?;"
		<<npcInstanceID
		>>npcID;
		return npcID;
	} catch(sqlite_exception e) {
		return 0;
	}
}

bool DatabaseTool::addNpc(
		 	int npcID, 
		 	string description, 
		 	vector<string> keywords,
		 	string longdesc,
		 	string shortdesc
		 	){
	string sqlStatment = "INSERT INTO npcs VALUES (" 
		+ to_string(npcID) 
		+ ", " + quotesql(description)
		+ ", " + quotesql(concatKeywords(keywords))
		+ ", " + quotesql(longdesc)
		+ ", " + quotesql(shortdesc) 
		+ ");";
	return executeSQLInsert(sqlStatment);

}

bool DatabaseTool::addZone(
		 	int zoneID,
		 	string zoneName,
		 	string description,
		 	vector<ExtendedDescription> extendedDescriptions,
		 	vector<Door> doors
		 	){
	try {
		database db(DB_LOCATION);
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
		return true;
	} catch(sqlite_exception e) {
		return false;
	}
}

string DatabaseTool::getZoneName(int zoneID){
	try {
		string zoneName = "";
		database db( DB_LOCATION );
		db <<"select zoneName from zones where zoneID=?;"
		<<zoneID
		>>zoneName;
		return zoneName;
	} catch(sqlite_exception e) {
		return "";
	}
}

string DatabaseTool::getZoneDesc(int zoneID){
	try {
		string description = "";
		database db( DB_LOCATION );
		db << "select zoneDescription from zones where zoneID=?;"
		<<zoneID
		>>description;
		return description;
	} catch(sqlite_exception e) {
		return "";
	}
}

string DatabaseTool::getZoneExtendedDesc(int zoneID, string keyword){
	try {
		string extendedDesc = "";
		database db( DB_LOCATION );
		db << "select keywords, description from zone_ext_descriptions where zoneID=?;"
		<<zoneID
		>>[&](string keywords, string description) {
			if(keywords.find(keyword) != string::npos) {
				extendedDesc = description;
			}
		};
		return extendedDesc;
	} catch(sqlite_exception e) {
		return "";
	}

}

int DatabaseTool::getDirectionID(int zoneID, string direction){
	try {
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
		return linksTo;
	} catch(sqlite_exception e) {
		cout << e.what() << endl;
		return 0;
	}
}

string DatabaseTool::getDirectionDesc(int zoneID, string direction){
	try {
		string doorDescription = "";
		database db( DB_LOCATION );
		boost::to_lower(direction);
		db << "select direction, keywords, description from doors where zoneID=?;"
		<<zoneID
		>>[&](string doorDirection, string keywords, string description) {
			if((keywords.find(direction) != string::npos) || (doorDirection == direction)) {
				doorDescription = description;
			}		
		};
		return doorDescription;
	} catch (sqlite_exception e) {
		cout << e.what() << endl;
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
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";

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
		return true;

	} catch(sqlite_exception e) {
		return false;
	}
}

vector<string> DatabaseTool::getItemsInInventory(int charID) {
	vector<string> items;
	try {
		database db(DB_LOCATION);
		db << "select shortDescription from items X, player_inventory Y where X.itemID = Y.itemID and charID =?"
		<<charID
		>>[&](string desc) {
			items.push_back(desc);
		};
		return items;
	} catch(sqlite_exception e) {
		cout << e.what() << endl;
		return items;
	}
}

vector<string> DatabaseTool::getItemsInZone(int zoneID) {
	vector<string> items;
	try {
		database db(DB_LOCATION);
		db << "select longDescription, containerID from items X, instanceOfItem Y where X.itemID = Y.itemID and zoneID =?"
		<<zoneID
		>>[&](string desc, int containerID) {
			if(containerID == 0) {
				items.push_back(desc);
			}
		};
		return items;
	} catch(sqlite_exception e) {
		cout << e.what() << endl;
		return items;

	}

}

bool DatabaseTool::addItem(Item item) {
	string sqlStatment = "INSERT INTO items VALUES ( " + to_string(item.itemID) + "," + quotesql(concatExtendedDescriptions(item.extendedDescriptions)) + "," + quotesql(concatKeywords(item.keywords)) + "," + quotesql(item.longDesc) + "," + quotesql(item.shortDesc) + ", 0, 0, 0);";
	return executeSQLInsert(sqlStatment);	
}


vector<string> DatabaseTool::getItemsInInventory(int charID) {
	vector<string> items;
	try {
		database db(DB_LOCATION);
		db << "select shortDesc from items X, instanceOfItem Y where X.itemID = Y.itemID and charID =?"
		<<charID
		>>[&](string desc) {
			items.push_back(desc);
		};
		return items;
	} catch(sqlite_exception e) {
		return items;
	}
}

vector<string> DatabaseTool::getItemsInZone(int zoneID) {
	vector<string> items;
	try {
		database db(DB_LOCATION);
		db << "select longDesc from items X, instanceOfItem Y where X.itemID = Y.itemID and zoneID =?"
		<<zoneID
		>>[&](string desc) {
			items.push_back(desc);
		};
		return items;
	} catch(sqlite_exception e) {
		return items;
	}

}

vector<int> DatabaseTool::getInstanceIDsOfItemsInInventory(int charID) {
	vector<int> items;
	try {
		database db(DB_LOCATION);
		db << "select itemInstanceID from instanceOfItem where charID =?"
		<<charID
		>>[&](int itemInstanceID) {
			items.push_back(itemInstanceID);
		};
		return items;
	} catch(sqlite_exception e) {
		return items;
	}
}

bool DatabaseTool::spawnItemInZone(int itemID, int zoneID){

	string sqlStatment = "INSERT INTO instanceOfItem VALUES ( NULL, " + to_string(itemID) + " , NULL , " + to_string(zoneID) + ", NULL, NULL, 0);";
	return executeSQLInsert(sqlStatment);

	try {
		database db(DB_LOCATION);
		db << "PRAGMA foreign_keys = ON;";
		db << "insert into instanceOfItem values (NULL, ?, ?, NULL)"
		<<itemID
		<<zoneID;
		return true;
	} catch(sqlite_exception e) {
		cout << e.what() << endl;
		return false;
	}
}

bool DatabaseTool::spawnItemInNpcInv(int itemID, int npcInstanceID){
	try {
		int canPickUp;
		database db (DB_LOCATION);

		db << "select canPickUp from items where itemID = ?;"
		<<itemID
		>>canPickUp;
		if(canPickUp > 0) {
			db << "INSERT INTO instanceOfItem VALUES ( NULL, ? , NULL , NULL, ?, NULL, 0);"
			<<itemID
			<<npcInstanceID;

		db << "PRAGMA foreign_keys = ON;";
		db << "select isPickable from items where itemID = ?;"
		<<itemID
		>>canPickUp;
		if(canPickUp > 0) {
			db << "INSERT INTO npc_inventory VALUES ( NULL, ?, ?, 0, 0);"
			<<npcInstanceID
			<<itemID;

			return true;
		} else {
			return false;
		}
	}catch(sqlite_exception e) {


		cout << e.what() << endl;
		return 0;
	}
}

bool DatabaseTool::spawnItemInCharacterInv(int itemID, int charID){
	try {
		int canPickUp;
		database db (DB_LOCATION);
		db << "select canPickUp from items where itemID = ?;"
		<<itemID
		>>canPickUp;
		if(canPickUp > 0) {
			db << "INSERT INTO instanceOfItem VALUES ( NULL, ? , ? , NULL, NULL, NULL, 0);"
			<<itemID
			<<charID;
		db << "PRAGMA foreign_keys = ON;";
		db << "select isPickable from items where itemID = ?;"
		<<itemID
		>>canPickUp;
		if(canPickUp > 0) {
			db << "INSERT INTO player_inventory VALUES ( NULL, ?, ?, 1, 0);"
			<<charID
			<<itemID;
			return true;
		} else {
			return false;
		}
	}catch(sqlite_exception e) {
		cout << e.what() << endl;
		return 0;
	}
}

bool DatabaseTool::spawnItemInItem(int itemID, int itemInstanceID) {
	try {
		int canPickUp;
		int isContainer;
		database db (DB_LOCATION);
		db << "select canPickUp from items where itemID = ?;"
		<<itemID
		>>canPickUp;

		db << "select isContainer from items X, instanceOfItem Y where X.itemID = Y.itemID and itemInstanceID = ?;"
		<<itemInstanceID
		>> isContainer;

		if((canPickUp > 0) && (isContainer > 0)) {
			db << "INSERT INTO instanceOfItem VALUES ( NULL, ? , NULL , NULL, NULL, ? , 0);"
			<<itemID
			<<itemInstanceID;
			return true;
		} else {
			return false;
		}
	}catch(sqlite_exception e) {
		cout << e.what() << endl;
		return 0;
	}
}


bool DatabaseTool::moveItem(int itemInstanceID, Transfer where, int toID){
	try {
		int canPickUp;
		int isContainer;
		database db(DB_LOCATION);
		db << "select canPickUp from items X, instanceOfItem Y where X.itemID = Y.itemID and itemInstanceID = ?;"
		<<itemInstanceID
		>>canPickUp;
		switch(where) {
			case toCharacter:
				if(canPickUp > 0) {
					db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, zoneID = NULL, npcInstanceID = NULL, charID = ? WHERE itemInstanceID = ?;"
					<<toID
					<<itemInstanceID;
					return true;
				} else {
					return false;
				}
			case toZone:
				db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, charID = NULL, npcInstanceID = NULL, zoneID = ? WHERE itemInstanceID = ?;"
				<<toID
				<<itemInstanceID;
				return true;
			case toNpc:
				if(canPickUp > 0) {
					db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, zoneID = NULL, npcInstanceID = NULL, charID = ? WHERE instanceID = ?;"
					<<toID
					<<itemInstanceID;
					return true;
				} else {
					return false;
				}
			case toItem:
				db << "select isContainer from items X, instanceOfItem Y where X.itemID = Y.itemID and itemInstanceID = ?;"
				<< toID
				>> isContainer;
				if((canPickUp > 0) && (isContainer >0)) {
					db << "UPDATE instanceOfItem SET otherItemInstanceID = NULL, charID = NULL, zoneID = NULL, npcInstanceID = NULL, otherItemInstanceID = ? WHERE itemInstanceID = ?;"
					<<toID
					<<itemInstanceID;
					return true;
				} else {
					return false;
				}
			default:
				return false;
		}
	} catch(sqlite_exception e) {
		return false;
		int zoneID;
		database db (DB_LOCATION);
		db << "PRAGMA foreign_keys = ON;";
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
			return true;
		} else {
			return false;
		}
	}catch(sqlite_exception e) {
		cout << e.what() << endl;
		return 0;
	}
}

bool DatabaseTool::deleteItem(int itemInstanceID){
	string statment = "delete from instanceOfItem where itemInstanceID=" + to_string(itemInstanceID) + ";";
	return executeSQLInsert(statment);
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
	string statment = "insert into resetCommands values (NULL,"
		+ quotesql(command.action) + ","
		+ to_string(command.id) + ","
		+ to_string(command.slot) + "," 
		+ to_string(command.npcLimit) + ","
		+ to_string(command.room) + ","
		+ quotesql(command.state) + ","
		+ to_string(command.container) + ");";
	return executeSQLInsert(statment);
}

Attributes DatabaseTool::getAttributes(int id, Target characterOrNpc){
	Attributes attributes;
	try {
		database db(DB_LOCATION);
		switch(characterOrNpc) {
			case character:
				db << "select charID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot, chestSlot, greavesSlot, feetSlot, handSlot, weponSlot from playerAttributes where charID = ?;"
				<< id
				>>[&](int charID, int level, int experience, int health, int strength, int intelligence, int dexterity, int charisma, int ringSlot, int headSlot, int chestSlot, int greavesSlot, int feetSlot, int handSlot, int weponSlot) {
					Attributes characterAttributes(charID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot,  chestSlot, greavesSlot, feetSlot, handSlot, weponSlot);
					attributes = characterAttributes;
				};
				return attributes;
				break;
			case npc:
				db << "select npcInstanceID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot, chestSlot, greavesSlot, feetSlot, handSlot, weponSlot from npcAttributes where npcInstanceID = ?;"
				<< id
				>>[&](int npcInstanceID, int level, int experience, int health, int strength, int intelligence, int dexterity, int charisma, int ringSlot, int headSlot, int chestSlot, int greavesSlot, int feetSlot, int handSlot, int weponSlot) {
					Attributes npcAttributes(npcInstanceID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot,  chestSlot, greavesSlot, feetSlot, handSlot, weponSlot);
					attributes = npcAttributes;
				};
				return attributes;
				break;
			default:
				return attributes;
		}
	} catch(sqlite_exception e) {
		return attributes;
	}
}

bool DatabaseTool::updateAttributes(Attributes attributes, Target target){
	switch(target) {
		case character:
			try {
				database db(DB_LOCATION);
				db << "UPDATE playerAttributes SET level = ?, experience = ?, health = ?, strength = ?, intelligence = ?, dexterity = ?, charisma = ?, ringSlot = ? , headSlot = ?, chestSlot = ?, greavesSlot = ?, feetSlot = ?, handSlot = ?, weponSlot = ? where charID = ?;"
				<<attributes.level
				<<attributes.experience
				<<attributes.health
				<<attributes.strength
				<<attributes.intelligence
				<<attributes.dexterity
				<<attributes.charisma
				<<attributes.ringSlot
				<<attributes.headSlot
				<<attributes.chestSlot
				<<attributes.greavesSlot
				<<attributes.feetSlot
				<<attributes.handSlot
				<<attributes.weponSlot
				<<attributes.id;
				return true;
			} catch(sqlite_exception e) {
				return false;
			}
		case npc:
			try {
				database db(DB_LOCATION);
				db << "UPDATE npcAttributes SET level = ?, experience = ?, health = ?, strength = ?, intelligence = ?, dexterity = ?, charisma = ?, ringSlot = ? , headSlot = ?, chestSlot = ?, greavesSlot = ?, feetSlot = ?, handSlot = ?, weponSlot = ? where npcInstanceID = ?;"
				<<attributes.level
				<<attributes.experience
				<<attributes.health
				<<attributes.strength
				<<attributes.intelligence
				<<attributes.dexterity
				<<attributes.charisma
				<<attributes.ringSlot
				<<attributes.headSlot
				<<attributes.chestSlot
				<<attributes.greavesSlot
				<<attributes.feetSlot
				<<attributes.handSlot
				<<attributes.weponSlot
				<<attributes.id;
				return true;
			} catch(sqlite_exception e) {
				return false;
			}
	}
}

bool DatabaseTool::equipItem(int charID, string item) {
	bool foundItem = false;
	try{
		database db (DB_LOCATION);
		db << "select shortDesc, keywords, itemInstanceID, equipableSlot from items X, instanceOfItem Y where X.itemID = Y.itemID and charID = ?;"
		<<charID
		>>[&](string shortdesc, string keywords, int itemInstanceID, int equipableSlot) {
			if((shortdesc.find(item) != string::npos) || (keywords.find(item) != string::npos)) {
				int currentlyEquiped;
				db << "select " + getSlot(equipableSlot) + " from playerAttributes where charID = ?;"
				<<charID
				>>currentlyEquiped;

				cout << "equipped: " << currentlyEquiped << endl;

				if(currentlyEquiped != 0) {
					cout << "testing" << endl;
					db <<"update instanceOfItem set isEquipped = 0 where itemInstanceID = ?;"
					<< currentlyEquiped;
				}
				db <<"update instanceOfItem set isEquipped = 1 where itemInstanceID = ?;"
				<<itemInstanceID;
				
				string statment = "update playerAttributes set " + getSlot(equipableSlot) + "= ?  where charID = ? ;";
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
		return foundItem;
	} catch(sqlite_exception e) {
		cout << e.what() << endl;
		return false;
	}

}

bool DatabaseTool::pickUp(int charID, string item) {
	bool foundItem = false;
	int foundItemId;
	try{
		database db (DB_LOCATION);
		db << "select shortDesc, keywords, itemInstanceID from items X, instanceOfItem Y, characters Z where X.itemID = Y.itemID and Z.location = Y.zoneID and Z.charID = ?;"
		<<charID
		>>[&](string shortdesc, string keywords, int itemInstanceID) {
			if((shortdesc.find(item) != string::npos) || (keywords.find(item) != string::npos)) {
				foundItem = true;
				foundItemId = itemInstanceID;
			}
		};
	int foundItemID;
	int foundInstanceID;
	int canPickUp;
	try{
		database db (DB_LOCATION);
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
			return true;
		} else {
			return false;
		}

	} catch(sqlite_exception e) {
		cout << e.what() << endl;
		return false;
	}
 
	cout << "test" << endl;
	if(foundItem) {
		moveItem(foundItemId, Transfer::toCharacter, charID);
	}
	return foundItem;
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


string DatabaseTool::look(int charID) {
bool DatabaseTool::setCombatFlag(int id, bool inCombat, Target characterOrNpc) {
	try {
		database db(DB_LOCATION);
		switch(characterOrNpc) {
			case character:
				if(inCombat) {
					db << "update charactersOnline set inCombat = 1 where charID =?"
					<<id;
				} else {
					db << "update charactersOnline set inCombat = 0 where charID =?"
					<<id;					
				}
				return true;
			case npc:
				if(inCombat) {
					db << "update instanceOfNpc set inCombat = 1 where npcInstanceID = ?"
					<< id;
				} else {
					db << "update instanceOfNpc set inCombat = 0 where npcInstanceID = ?"
					<< id;
					
				}
				return true;
			default:
				return false;
		}
	} catch(sqlite_exception e) {
		return false;
	}

}

bool DatabaseTool::inCombat(int id, Target characterOrNpc) {
	try {
		database db(DB_LOCATION);
		int inCombat;
		switch(characterOrNpc) {
			case character:
				db << "select inCombat from charactersOnline where charID = ?"
				<<id
				>>inCombat;
				return inCombat;
			case npc:
				db << "select inCombat from instanceOfNpc where npcInstanceID = ?"
				<<id
				>>inCombat;
				return inCombat;
		}
	} catch (sqlite_exception e) {
		return false;
	}
}

int DatabaseTool::getNpcInstanceIDFromName(string name, int zoneID) {
	try {
		int instanceID = 0;
		database db(DB_LOCATION);
		db << "select npcInstanceID, keywords, shortdesc from instanceOfNpc X, npcs Y where X.npcID = Y.npcID and zoneID = ?"
		<<zoneID
		>>[&](int npcInstanceID, string keywords, string shortDesc) {
			if((keywords.find(name) != string::npos) || (shortDesc.find(name) != string::npos)) {
				instanceID = npcInstanceID;
			}
		};
		return instanceID;
	} catch(sqlite_exception e) {
		return 0;
	}

}




// // NEW STUFF by pavel

int DatabaseTool::createNewZone( string zoneName, string zoneDesc ) {
	try {
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";

		db 	<< "INSERT INTO zones (zoneName,zoneDescription) VALUES (?,?);"
			<< zoneName
			<< zoneDesc;

		return db.last_insert_rowid();
	} catch ( exception& e ) {
		return 0;
	}
}


int DatabaseTool::createNewZone( int zoneID, string zoneName, string zoneDesc ) {
	try {
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";

		db 	<< "INSERT INTO zones (zoneID,zoneName,zoneDescription) VALUES (?,?,?);"
			<< zoneID
			<< zoneName
			<< zoneDesc;

		return db.last_insert_rowid();
	} catch ( exception& e ) {
		return 0;
	}
}


bool DatabaseTool::addExtendedDescriptionToZone( int zoneID, string desc, string keywords ) {
	try {
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";

		db 	<< "INSERT INTO zone_ext_descriptions (zoneID,description,keywords) VALUES (?,?,?);"
			<< zoneID
			<< desc
			<< keywords;

		return true;
	} catch ( exception& e ) {
		return false;
	}
}


bool DatabaseTool::addDoorToZone( int zoneID, string description, string direction, int pointer, string keywords ) {
	try {
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";

		db 	<< "INSERT INTO doors (zoneID,description,keywords,direction,linksTo) VALUES (?,?,?,?,?);"
			<< zoneID
			<< description
			<< keywords
			<< direction
			<< pointer;

		return true;
	} catch ( exception& e ) {
		return false;
	}
}


string DatabaseTool::getDoorDescriptionAt( int zoneID, string direction ) {
	try {
		database db(DB_LOCATION);

		string fetchedDesc;
		db 	<< "SELECT description FROM doors WHERE zoneID == ? AND direction == ?;"
			<< zoneID
			<< direction
			>> fetchedDesc;

		return fetchedDesc;
	} catch ( exception& e ) {
		return "There is nothing there.";
	}
}


int DatabaseTool::getZoneIDBehindDoorAt( int zoneID, string direction ) {
	try {
		database db(DB_LOCATION);

		int fetchedZoneID;
		db 	<< "SELECT linksTo FROM doors_n WHERE zoneID == ? AND direction == ?;"
			<< zoneID
			<< direction
			>> fetchedZoneID;

		return fetchedZoneID;
	} catch ( exception& e ) {
		return 0;
	}
}


bool DatabaseTool::moveCharacterToZone( int charID, int zoneID ) {
	try {
		database db(DB_LOCATION);

		db 	<< "UPDATE characters SET location = ? WHERE charID == ?;"
			<< zoneID
			<< charID;

		return true;
	} catch ( exception& e ) {
		return false;
	}
}












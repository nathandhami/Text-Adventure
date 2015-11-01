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
const string INITIAL_ZONE = "3001";
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
	string sqlStatment = "INSERT INTO charactersOnline VALUES ( " + to_string(charID) + " , " + quotesql(sessionID) + ");";
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

bool DatabaseTool::placeNpcInZone(int npcID, int zoneID){
	try {
		database db(DB_LOCATION);
		db << "PRAGMA foreign_keys = ON;";
		db << "INSERT INTO instanceOfNpc VALUES ( NULL, ?, ?, 1);"
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

vector<int> DatabaseTool::getAllNpcsInZone(int zoneID){
	vector<int> npcsInZone;
	database db( DB_LOCATION );
	db << "select npcInstanceID from instanceOfNpc where zoneID=? and isAlive = 1;"
	<< zoneID
	>>[&](int npcID) {
		npcsInZone.push_back(npcID);
	};
	return npcsInZone;
}

void DatabaseTool::removeNpcFromZone(int npcInstanceID, int zoneID){
	string sqlStatment = "delete from instanceOfNpc where npcID=" + to_string(npcInstanceID) + " and " + "zoneID =" + to_string(zoneID) + ";";
	executeSQLInsert(sqlStatment);
}



string DatabaseTool::getNPCDesc(int npcID){
	try {
		string description;
		database db( DB_LOCATION );
		db << "select description from npcs where npcID=?;"
		<<npcID
		>>description;
		return description;
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

bool DatabaseTool::addNPC(
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
	string sqlStatment = "INSERT INTO zones VALUES (" + to_string(zoneID) 
			+ ", " + quotesql(zoneName) 
			+ ", " + quotesql(description) 
			+ ", " + quotesql(concatExtendedDescriptions(extendedDescriptions))
			+ ", " + quotesql(concatDoors(doors))
			+ ");";
	return executeSQLInsert(sqlStatment);
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
		db << "select description from zones where zoneID=?;"
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
		db << "select extendedDesc from zones where zoneID=?;"
		<<zoneID
		>>extendedDesc;
		return parseExtendedDesc(extendedDesc, keyword);
	} catch(sqlite_exception e) {
		return "";
	}

}

int DatabaseTool::getDirectionID(int zoneID, string direction){
	try {
		string doors = "";
		database db( DB_LOCATION );
		boost::to_lower(direction);
		db << "select doors from zones where zoneID=?;"
		<<zoneID
		>>doors;
		return parseDirectionID(doors, direction);
	} catch(sqlite_exception e) {
		return 0;
	}
}

string DatabaseTool::getDirectionDesc(int zoneID, string direction){
	try {
		string doors = "";
		database db( DB_LOCATION );
		boost::to_lower(direction);
		db << "select doors from zones where zoneID=?;"
		<<zoneID
		>>doors;
		return parseDirectionDesc(doors, direction);
	} catch (sqlite_exception e) {
		return "";
	}
}

string DatabaseTool::parseExtendedDesc(string extendedDesc, string keyword){
	string findThis = "desc:";

	vector<string> singleChunk;
	size_t found = extendedDesc.find(findThis);
	while(found != string::npos) {
		size_t secondFound = extendedDesc.find(findThis, found + 1);
		string singleExtendedDesc = extendedDesc.substr(found, secondFound);
		boost::erase_all(singleExtendedDesc, findThis);
		singleChunk.push_back(singleExtendedDesc);
		found = secondFound;
	}

	int descIndex = -1;
	for(int i = 0; i < singleChunk.size(); i++) {
		if(singleChunk[i].find(keyword) != string::npos) {
			descIndex = i;
		}
	}
	if(descIndex == -1) {
		return "";
	} else {
		return singleChunk[descIndex].substr(0, singleChunk[descIndex].find("keywords:"));
	}
}

int DatabaseTool::parseDirectionID(string doors, string direction) {
	string findThis = "desc:";

	vector<string> singleDoor;
	size_t found = doors.find(findThis);
	while(found != string::npos) {
		size_t secondFound = doors.find(findThis, found + 1);
		string singleExtendedDesc = doors.substr(found, secondFound);
		boost::erase_all(singleExtendedDesc, findThis);
		singleDoor.push_back(singleExtendedDesc);
		found = secondFound;
	}

	int doorIndex = -1;
	for(int i = 0; i < singleDoor.size(); i++) {
		if(singleDoor[i].find(direction) != string::npos){
			doorIndex = i;
		}
	}
	
	if(doorIndex == -1) {
		return 0;
	} else {
		string directionID = singleDoor[doorIndex].substr(singleDoor[doorIndex].find("to:"));
		boost::erase_all(directionID, "to: ");
		return atoi(directionID.c_str());
	}
}

string DatabaseTool::parseDirectionDesc(string doors, string direction) {
	string findThis = "desc:";

	vector<string> singleDoor;
	size_t found = doors.find(findThis);
	while(found != string::npos) {
		size_t secondFound = doors.find(findThis, found + 1);
		string singleExtendedDesc = doors.substr(found, secondFound);
		boost::erase_all(singleExtendedDesc, findThis);
		singleDoor.push_back(singleExtendedDesc);
		found = secondFound;
	}

	int doorIndex = -1;
	for(int i = 0; i < singleDoor.size(); i++) {
		if(singleDoor[i].find(direction) != string::npos){
			doorIndex = i;
		}
	}
	
	if(doorIndex == -1) {
		return "";
	} else {
		string directionDesc = singleDoor[doorIndex].substr(0, singleDoor[doorIndex].find("dir:"));
		boost::erase_all(directionDesc, "- ");
		boost::algorithm::trim( directionDesc );
		return directionDesc;
	}
}

string DatabaseTool::concatDoors(vector<Door> doors) {
	string combinedDoors = "";
	for(auto& door: doors) {
		combinedDoors = combinedDoors + " desc: " + door.description + " dir: " + door.direction + " keywords: " + concatKeywords(door.keywords) + "to: " + to_string(door.goesTo);
	}
	return combinedDoors;
}

string DatabaseTool::concatExtendedDescriptions(vector<ExtendedDescription> extendedDescriptions) {
	string combinedDescriptions = "";
	for(auto& extendedDescription: extendedDescriptions) {
		combinedDescriptions = combinedDescriptions + " desc: " + extendedDescription.description + " keywords: " + concatKeywords(extendedDescription.keywords);
	}
	return combinedDescriptions;
}

string DatabaseTool::concatKeywords(vector<string> keywords) {
	string combinedKeywords = "";
	for(auto& keyword: keywords) {
		combinedKeywords = combinedKeywords + keyword + " ";
	}
	return combinedKeywords;

}

bool DatabaseTool::addItem(Item item) {
	string sqlStatment = "INSERT INTO items VALUES ( " + to_string(item.itemID) + "," + quotesql(concatExtendedDescriptions(item.extendedDescriptions)) + "," + quotesql(concatKeywords(item.keywords)) + "," + quotesql(item.longDesc) + "," + quotesql(item.shortDesc) + ");";
	return executeSQLInsert(sqlStatment);	
}

bool DatabaseTool::spawnItemInZone(int itemID, int zoneID){
	string sqlStatment = "INSERT INTO instanceOfItem VALUES ( NULL, " + to_string(itemID) + " , NULL , " + to_string(zoneID) + ", NULL, NULL);";
	return executeSQLInsert(sqlStatment);
}

bool DatabaseTool::spawnItemInNpcInv(int itemID, int npcInstanceID){
	string sqlStatment = "INSERT INTO instanceOfItem VALUES ( NULL, " + to_string(itemID) + " , NULL , NULL, " + to_string(npcInstanceID) + ", NULL);";
	return executeSQLInsert(sqlStatment);
}

bool DatabaseTool::spawnItemInCharacterInv(int itemID, int charID){
	string sqlStatment = "INSERT INTO instanceOfItem VALUES ( NULL, " + to_string(itemID) + " , " + to_string(charID) + ", NULL, NULL, NULL);";
	return executeSQLInsert(sqlStatment);
}

bool DatabaseTool::spawnItemInItem(int itemID, int itemInstanceID) {
	string sqlStatment = "INSERT INTO instanceOfItem VALUES ( NULL, " + to_string(itemID) + " , NULL , NULL, NULL, " + to_string(itemInstanceID) + ");";
	return executeSQLInsert(sqlStatment);
}


bool DatabaseTool::moveItem(int instanceID, Transfer where, int toID){
	string sqlStatment = "";
	switch(where) {
		case toCharacter:
			sqlStatment = "UPDATE instanceOfItem SET otherItemInstanceID = NULL, zoneID = NULL, npcInstanceID = NULL, charID = " + to_string(toID) + " WHERE instanceID = " + to_string(instanceID) + ";";
			return executeSQLInsert(sqlStatment);
		case toZone:
			sqlStatment = "UPDATE instanceOfItem SET otherItemInstanceID = NULL, charID = NULL, npcInstanceID = NULL, zoneID = " + to_string(toID) + " WHERE instanceID = " + to_string(instanceID) + ";";
			return executeSQLInsert(sqlStatment);
		case toNpc:
			sqlStatment = "UPDATE instanceOfItem SET otherItemInstanceID = NULL, charID = NULL, zoneID = NULL, npcInstanceID = " + to_string(toID) + " WHERE instanceID = " + to_string(instanceID) + ";";
			return executeSQLInsert(sqlStatment);
		case toItem:
			sqlStatment = "UPDATE instanceOfItem SET otherItemInstanceID = NULL, charID = NULL, zoneID = NULL, npcInstanceID = NULL, otherItemInstanceID = " + to_string(toID) + " WHERE instanceID = " + to_string(instanceID) + ";";
			return executeSQLInsert(sqlStatment);
		default:
			return false;
	}

}

bool DatabaseTool::deleteItem(int instanceID){
	string statment = "delete from instanceOfItem where instanceID=" + to_string(instanceID) + ";";
	return executeSQLInsert(statment);
}

bool DatabaseTool::addResetCommand(ResetCommand command){
	string statment = "insert into resetCommands values ((SELECT IFNULL(MAX(resetID), 0) + 1 FROM resetCommands),"
		+ quotesql(command.action) + ","
		+ to_string(command.id) + ","
		+ to_string(command.slot) + "," 
		+ to_string(command.npcLimit) + ","
		+ to_string(command.room) + ");";
	return executeSQLInsert(statment);
}

Attributes DatabaseTool::getAttributes(int id, Target target){
	switch(target) {
		case character:
			try {
				database db(DB_LOCATION);
				db << "select charID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot, chestSlot, greavesSlot, feetSlot, handSlot, weponSlot from playerAttributes where charID = ?;"
				<< id
				>>[&](int charID, int level, int experience, int health, int strength, int intelligence, int dexterity, int charisma, int ringSlot, int headSlot, int chestSlot, int greavesSlot, int feetSlot, int handSlot, int weponSlot) {
					Attributes attributes(charID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot,  chestSlot, greavesSlot, feetSlot, handSlot, weponSlot);
					return attributes;
				};
			} catch(sqlite_exception e) {
				Attributes emptyAttributes;
				return emptyAttributes;
			}
		case npc:
			try {
				database db(DB_LOCATION);
				db << "select npcInstanceID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot, chestSlot, greavesSlot, feetSlot, handSlot, weponSlot from npcAttributes where npcInstanceID = ?;"
				<< id
				>>[&](int npcInstanceID, int level, int experience, int health, int strength, int intelligence, int dexterity, int charisma, int ringSlot, int headSlot, int chestSlot, int greavesSlot, int feetSlot, int handSlot, int weponSlot) {
					Attributes attributes(npcInstanceID, level, experience, health, strength, intelligence, dexterity, charisma, ringSlot, headSlot,  chestSlot, greavesSlot, feetSlot, handSlot, weponSlot);
					return attributes;
				};
			} catch(sqlite_exception e) {
				Attributes emptyAttributes;
				return emptyAttributes;
			}
		default:
			Attributes emptyAttributes;
			return emptyAttributes;
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

string DatabaseTool::look(int charID) {

}








// NEW STUFF by pavel

int DatabaseTool::createNewZone( string zoneName, string zoneDesc ) {
	try {
		database db(DB_LOCATION);

		db << "PRAGMA foreign_keys = ON;";

		db 	<< "INSERT INTO zones_n (zoneName,zoneDescription) VALUES (?,?);"
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

		db 	<< "INSERT INTO doors_n (zoneID,description,keywords,direction,linksTo) VALUES (?,?,?,?,?);"
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


string DatabaseTool::getDoorDescription( int zoneID, string direction ) {
	try {
		database db(DB_LOCATION);

		string fetchedDesc;
		db 	<< "SELECT description FROM doors_n WHERE zoneID == ? AND direction == ?;"
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












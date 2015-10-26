#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <sstream>
#include "DatabaseTool.hpp"
#include "Database.h"
#include "Query.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

using namespace std;

const string DB_LOCATION = "apps/server/databases/adventureDB";
const string INITIAL_ZONE = "9999";
const int PLAYER_OFFLINE = 0;
const int PLAYER_ONLINE = 1;


string DatabaseTool::quotesql( const string& s ) {
	if(s.empty()) {
		return string("NULL");
	} else {
    	return string("'") + s + string("'");
	}
}

bool DatabaseTool::executeSQLInsert(string statment){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	return query.execute(statment.c_str());

}

bool DatabaseTool::addUser(string userName, string password) {
	string sqlStatment = "INSERT INTO users VALUES ( NULL, " + quotesql(userName) + "," + quotesql(password) + ");";
	return executeSQLInsert(sqlStatment);
}

int DatabaseTool::getUserID(string userName, string password){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select userID from users where userName=" + quotesql(userName) + " AND password=" + quotesql(password) + ";";
	int charID = (int) query.get_count(sqlStatment.c_str());
	return charID;
}

string DatabaseTool::getPassword(int userID) {
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select password from users where userID=" + to_string(userID) +";";
	string password = query.get_string(sqlStatment.c_str());
	return password;

}

bool DatabaseTool::addCharacter(string name, int userID){
	string sqlStatment = "INSERT INTO characters VALUES ( NULL, " + quotesql(name) + "," + to_string(userID) + "," + INITIAL_ZONE + "," + to_string(PLAYER_OFFLINE) + ");";
	return executeSQLInsert(sqlStatment);
}

bool DatabaseTool::isCharOnline(int charID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "SELECT EXISTS(SELECT 1 FROM charactersOnline WHERE charID=" + to_string(charID) + " LIMIT 1);";
	int onlineStatus = (int) query.get_count(sqlStatment.c_str());
	if(onlineStatus == 1) {
		return true;
	} else {
		return false;
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
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select sessionID from charactersOnline where charID=" + to_string(charID) +";";
	string sessionID = query.get_string(sqlStatment.c_str());
	return sessionID;

}

int DatabaseTool::getCharID(int userID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select charID from characters where userID=" + to_string(userID) +";";
	int charID = (int) query.get_count(sqlStatment.c_str());
	return charID;
}

void DatabaseTool::putCharInZone(int charID, int zoneID){
	string sqlStatment = "UPDATE characters SET location = " + to_string(zoneID) + " WHERE charID = " + to_string(charID) + ";";
	executeSQLInsert(sqlStatment);
}

int DatabaseTool::getCharsLocation(int charID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select location from characters where charID=" + to_string(charID) + ";";
	int zoneID = (int) query.get_count(sqlStatment.c_str());
	return zoneID;
}

vector<int> DatabaseTool::getAllCharsInZone(int zoneID){
	vector<int> charsInZone;
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select charID from characters where location=" + to_string(zoneID) + ";";
	//TODO: only return characters that are currently online
	//" and isOnline =" + to_string(PLAYER_ONLINE) + ";";
	query.get_result(sqlStatment.c_str());
	while(query.fetch_row()) {
		int character = (int) query.getval();
		charsInZone.push_back(character);
	}
	query.free_result();
	return charsInZone;

}

void DatabaseTool::placeNpcInZone(int npcID, int zoneID){
	string sqlStatment = "INSERT INTO instanceOfNpc VALUES ( NULL, " + to_string(npcID) + "," + to_string(zoneID) + ");";
	executeSQLInsert(sqlStatment);
}

vector<int> DatabaseTool::getAllNpcsInZone(int zoneID){
	vector<int> npcsInZone;
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select npcID from instanceOfNpc where zoneID=" + to_string(zoneID) + ";";
	query.get_result(sqlStatment.c_str());
	while(query.fetch_row()) {
		int npc = (int) query.getval();
		npcsInZone.push_back(npc);
	}
	query.free_result();
	return npcsInZone;
}

void DatabaseTool::removeNpcFromZone(int npcInstanceID, int zoneID){
	string sqlStatment = "delete from instanceOfNpc where npcID=" + to_string(npcInstanceID) + " and " + "zoneID =" + to_string(zoneID) + ";";
	executeSQLInsert(sqlStatment);
}

string DatabaseTool::getNPCDesc(int npcID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select description from npcs where npcID=" + to_string(npcID) +";";
	string description = query.get_string(sqlStatment.c_str());
	return description;
}

void DatabaseTool::addNPC(
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
	executeSQLInsert(sqlStatment);

}

void DatabaseTool::addZone(
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
	executeSQLInsert(sqlStatment);
}

string DatabaseTool::getZoneName(int zoneID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select zoneName from zones where zoneID=" + to_string(zoneID) +";";
	string zoneName = query.get_string(sqlStatment.c_str());
	return zoneName;
}

string DatabaseTool::getZoneDesc(int zoneID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select description from zones where zoneID=" + to_string(zoneID) +";";
	string description = query.get_string(sqlStatment.c_str());
	return description;
}

string DatabaseTool::getZoneExtendedDesc(int zoneID, string keyword){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	string sqlStatment = "select extendedDesc from zones where zoneID=" + to_string(zoneID) +";";
	string extendedDesc = query.get_string(sqlStatment.c_str());
	
	string parsedDesc = parseExtendedDesc(extendedDesc, keyword);
	return parsedDesc;

}

int DatabaseTool::getDirectionID(int zoneID, string direction){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	boost::to_lower(direction);
	string sqlStatment = "select doors from zones where zoneID=" + to_string(zoneID) + ";";
	string doors = query.get_string(sqlStatment.c_str());
	int directionID = parseDirectionID(doors, direction);
	return directionID;
}

string DatabaseTool::getDirectionDesc(int zoneID, string direction){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query query(db);
	boost::to_lower(direction);
	string lowerCaseDirection = direction;
	string sqlStatment = "select doors from zones where zoneID=" + to_string(zoneID) + ";";
	string doors = query.get_string(sqlStatment.c_str());
	return parseDirectionDesc(doors, direction);
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
	string sqlStatment = "INSERT INTO items VALUES ( " + to_string(item.itemID) + "," + quotesql(concatExtendedDescriptions(item.extendedDescriptions)) + "," + quotesql(concatKeywords(item.keywords)) + "," + quotesql(item.description) + ");";
	return executeSQLInsert(sqlStatment);	
}



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
	string sqlStatment = "select isOnline from characters where userID=" + to_string(charID) +";";
	int onlineStatus = (int) query.get_count(sqlStatment.c_str());
	if(onlineStatus == PLAYER_ONLINE) {
		return true;
	} else {
		return false;
	}
}

void DatabaseTool::setCharOnline(int charID){
	string sqlStatment = "UPDATE characters SET isOnline = " + to_string(PLAYER_ONLINE) + " WHERE charID = " + to_string(charID) + ";";
	executeSQLInsert(sqlStatment);
}

void DatabaseTool::setCharOffline(int charID){
	string sqlStatment = "UPDATE characters SET isOnline = " + to_string(PLAYER_OFFLINE) + " WHERE charID = " + to_string(charID) + ";";
	executeSQLInsert(sqlStatment);
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
	string sqlStatment = "select charID from characters where location=" + to_string(zoneID) + " and isOnline =" + to_string(PLAYER_ONLINE) + ";";
	query.get_result(sqlStatment.c_str());
	while(query.fetch_row()) {
		int character = (int) query.getval();
		charsInZone.push_back(character);
	}
	query.free_result();
	return charsInZone;

}

void DatabaseTool::placeNpcInZone(int npcID, int zoneID){
	string sqlStatment = "INSERT INTO populated_by VALUES (" + to_string(npcID) + "," + to_string(zoneID) + ");";
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
	string sqlStatment = "select npcID from populated_by where zoneID=" + to_string(zoneID) + ";";
	query.get_result(sqlStatment.c_str());
	while(query.fetch_row()) {
		int npc = (int) query.getval();
		npcsInZone.push_back(npc);
	}
	query.free_result();
	return npcsInZone;
}

void DatabaseTool::removeNpcFromZone(int npcID, int zoneID){
	string sqlStatment = "delete from populated_by where npcID=" + to_string(npcID) + " and " + "zoneID =" + to_string(zoneID) + ";";
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
		 	string keywords,
		 	string longdesc,
		 	string shortdesc
		 	){
	string sqlStatment = "INSERT INTO npcs VALUES (" 
		+ to_string(npcID) 
		+ ", " + quotesql(description)
		+ ", " + quotesql(keywords)
		+ ", " + quotesql(longdesc)
		+ ", " + quotesql(shortdesc) 
		+ ");";
	executeSQLInsert(sqlStatment);

}

void DatabaseTool::addZone(
		 	int zoneID,
		 	string zoneName,
		 	string description,
		 	string extendedDesc,
		 	int northID,
		 	string northDesc,
		 	int southID,
		 	string southDesc,
		 	int eastID,
		 	string eastDesc,
		 	int westID,
		 	string westDesc,
		 	int upID,
		 	string upDesc,
		 	int downID,
		 	string downDesc
		 	){
	string sqlStatment = "INSERT INTO zones VALUES (" + to_string(zoneID) 
			+ ", " + quotesql(zoneName) 
			+ ", " + quotesql(description) 
			+ ", " + quotesql(extendedDesc)
			+ ", " + to_string(northID)
			+ ", " + quotesql(northDesc)
			+ ", " + to_string(southID)
			+ ", " + quotesql(southDesc)
			+ ", " + to_string(eastID)
			+ ", " + quotesql(eastDesc)
			+ ", " + to_string(westID)
			+ ", " + quotesql(westDesc)
			+ ", " + to_string(upID)
			+ ", " + quotesql(upDesc)
			+ ", " + to_string(downID)
			+ ", " + quotesql(downDesc)
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
	string lowerCaseDirection = direction;
	string sqlStatment = "select " + lowerCaseDirection + "ID from zones where zoneID=" + to_string(zoneID) + ";";
	int directionID = (int) query.get_count(sqlStatment.c_str());
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
	string sqlStatment = "select " + lowerCaseDirection + "Desc from zones where zoneID=" + to_string(zoneID) + ";";
	string directionDesc = query.get_string(sqlStatment.c_str());
	return directionDesc;
}

string DatabaseTool::parseExtendedDesc(string extendedDesc, string keyword){
	string findThis = "- desc:\n";
	vector<vector<string>> parsedDesc;

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
		vector<string> singleExtendedDesc;
		string line;
		string description = "";
		bool foundKeyword = false;
		istringstream iss(singleChunk[i]);
		while(getline(iss, line)) {
			boost::erase_all(line, "- ");
			boost::algorithm::trim( line );
			if((line.find("keywords:") == string::npos) && (foundKeyword == false)){
				description = description + line;
			} else if((line.find("keywords:") != string::npos) && (foundKeyword == false)) {
				singleExtendedDesc.push_back(description);
				foundKeyword = true;
			} else if(!line.empty() && line.find(keyword)){
				descIndex = i;
			}
		}
		parsedDesc.push_back(singleExtendedDesc);
	}
	if(descIndex == -1) {
		return "";
	} else {
		return parsedDesc[descIndex][0];
	}
}




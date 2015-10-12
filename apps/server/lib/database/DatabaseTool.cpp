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

void DatabaseTool::executeSQLInsert(string statment){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	q.execute(statment.c_str());

}

void DatabaseTool::addUser(string userName, string password) {
	string sqlStatment = "INSERT INTO users VALUES ( NULL, " + quotesql(userName) + "," + quotesql(password) + ");";
	executeSQLInsert(sqlStatment);
}

int DatabaseTool::getUserID(string userName, string password){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select userID from users where userName=" + quotesql(userName) + " AND password=" + quotesql(password) + ";";
	int charID = (int) q.get_count(sqlStatment.c_str());
	return charID;
}

string DatabaseTool::getPassword(int userID) {
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select password from users where userID=" + to_string(userID) +";";
	string password = q.get_string(sqlStatment.c_str());
	return password;

}

void DatabaseTool::addCharacter(string name, int userID){
	string sqlStatment = "INSERT INTO characters VALUES ( NULL, " + quotesql(name) + "," + to_string(userID) + "," + INITIAL_ZONE + "," + to_string(PLAYER_OFFLINE) + ");";
	executeSQLInsert(sqlStatment);
}

bool DatabaseTool::isCharOnline(int charID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select isOnline from characters where userID=" + to_string(charID) +";";
	int onlineStatus = (int) q.get_count(sqlStatment.c_str());
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
	Query q(db);
	string sqlStatment = "select charID from characters where userID=" + to_string(userID) +";";
	int charID = (int) q.get_count(sqlStatment.c_str());
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
	Query q(db);
	string sqlStatment = "select location from characters where charID=" + to_string(charID) + ";";
	int zoneID = (int) q.get_count(sqlStatment.c_str());
	return zoneID;
}


vector<int> DatabaseTool::getAllCharsInZone(int zoneID){
	vector<int> charsInZone;
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select charID from characters where location=" + to_string(zoneID) + " and isOnline =" + to_string(PLAYER_ONLINE) + ";";
	q.get_result(sqlStatment.c_str());
	while(q.fetch_row()) {
		int character = (int) q.getval();
		charsInZone.push_back(character);
	}
	q.free_result();
	return charsInZone;

}

void DatabaseTool::placeNpcInZone(int npcID, int zoneID){
//TODO:implement
}

vector<int> DatabaseTool::getAllNpcsInZone(int zoneID){
//TODO:implement
}

void DatabaseTool::removeNpcFromZone(int npcID, int zone){
//TODO:implement
}

string DatabaseTool::getNPCDesc(int npcID){
//TODO:implement
}

void DatabaseTool::addNPC(
		 	int npcID, 
		 	string description, 
		 	string keywords,
		 	string longdesc,
		 	string shortdesc
		 	){
//TODO:implement
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
			+ "," + quotesql(zoneName) 
			+ "," + quotesql(description) 
			+ "," + quotesql(extendedDesc)
			+ "," + to_string(northID)
			+ "," + quotesql(northDesc)
			+ "," + to_string(southID)
			+ "," + quotesql(southDesc)
			+ "," + to_string(eastID)
			+ "," + quotesql(eastDesc)
			+ "," + to_string(westID)
			+ "," + quotesql(westDesc)
			+ "," + to_string(upID)
			+ "," + quotesql(upDesc)
			+ "," + to_string(downID)
			+ "," + quotesql(downDesc)
			+ ");";
	executeSQLInsert(sqlStatment);
}

string DatabaseTool::getZoneName(int zoneID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select zoneName from zones where zoneID=" + to_string(zoneID) +";";
	string zoneName = q.get_string(sqlStatment.c_str());
	return zoneName;
}

string DatabaseTool::getZoneDesc(int zoneID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select description from zones where zoneID=" + to_string(zoneID) +";";
	string description = q.get_string(sqlStatment.c_str());
	return description;
}

vector<vector<string>> DatabaseTool::getZoneExtendedDesc(int zoneID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select extendedDesc from zones where zoneID=" + to_string(zoneID) +";";
	string extendedDesc = q.get_string(sqlStatment.c_str());
	
	vector<vector<string>> parsedDesc = parseExtendedDesc(extendedDesc);
	return parsedDesc;

}

int DatabaseTool::getDirectionID(int zoneID, string direction){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	boost::to_lower(direction);
	string lowerCaseDirection = direction;
	string sqlStatment = "select " + lowerCaseDirection + "ID from zones where zoneID=" + to_string(zoneID) + ";";
	int directionID = (int) q.get_count(sqlStatment.c_str());
	return directionID;

}

string DatabaseTool::getDirectionDesc(int zoneID, string direction){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	boost::to_lower(direction);
	string lowerCaseDirection = direction;
	string sqlStatment = "select " + lowerCaseDirection + "Desc from zones where zoneID=" + to_string(zoneID) + ";";
	string directionDesc = q.get_string(sqlStatment.c_str());
	return directionDesc;
}

vector<vector<string>> DatabaseTool::parseExtendedDesc(string extendedDesc){
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

	for(int i = 0; i < singleChunk.size(); i++) {
		vector<string> singleExtendedDesc;
		string line;
		string description = "";
		bool keywordFlag = false;
		istringstream iss(singleChunk[1]);
		while(getline(iss, line)) {
			boost::erase_all(line, "- ");
			boost::algorithm::trim( line );
			if((line.find("keywords:") == string::npos) && (keywordFlag == false)){
				description = description + line;
			} else if((line.find("keywords:") != string::npos) && (keywordFlag == false)) {
				singleExtendedDesc.push_back(description);
				keywordFlag = true;
			} else {
				singleExtendedDesc.push_back(line);
			}
		}
		parsedDesc.push_back(singleExtendedDesc);
	}
	return parsedDesc;


}




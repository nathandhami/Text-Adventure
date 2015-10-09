#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "DatabaseTool.hpp"
#include "Database.h"
#include "Query.h"

using namespace std;

const string DB_LOCATION = "../../apps/server/databases/adventureDB";
const string INITIAL_ZONE = "9999";
const int PLAYER_OFFLINE = 0;
const int PLAYER_ONLINE = 1;
enum Direction = {NORTH, EAST, SOUTH, WEST};


string DatabaseTool::quotesql( const string& s ) {
    return string("'") + s + string("'");
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

void DatabaseTool::addUser(string userID, string password) {
	string sqlStatment = "INSERT INTO users VALUES (" + quotesql(userID) + "," + quotesql(password) + ");";
	executeSQLInsert(sqlStatment);
}

string DatabaseTool::getPassword(string userID) {
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select password from users where userID=" + quotesql(userID) +";";
	string password = q.get_string(sqlStatment.c_str());
	return password;

}

void DatabaseTool::addCharacter(string name, string userID){
	//adds character only if user has no other character
	if(getCharID(userID) == 0) {
		string sqlStatment = "INSERT INTO characters VALUES ( NULL, " + quotesql(name) + "," + quotesql(userID) + "," + INITIAL_ZONE + "," + to_string(PLAYER_OFFLINE) + ");";
		executeSQLInsert(sqlStatment);
	}

}

int DatabaseTool::getCharID(string userID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select charID from characters where userID=" + quotesql(userID) +";";
	int charID = (int) q.get_count(sqlStatment.c_str());
	return charID;

}

void DatabaseTool::putCharInZone(int charID, int zoneID){
	string sqlStatment = "UPDATE characters SET lastLocation = " + to_string(zoneID) + " WHERE charID = " + to_string(charID) + ";";
	executeSQLInsert(sqlStatment);

}

int DatabaseTool::getCharsLocation(int charID){
	Database db( DB_LOCATION );
	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "select lastLocation from characters where charID=" + to_string(charID) + ";";
	int zoneID = (int) q.get_count(sqlStatment.c_str());
	return zoneID;

}


vector<int> DatabaseTool::getAllCharsInZone(int zoneID){

}

void DatabaseTool::placeNpcInZone(int npcID, int zoneID){

}

vector<int> DatabaseTool::getAllNpcsInZone(int zoneID){

}

void DatabaseTool::removeNpcFromZone(int npcID, int zone){

}

string DatabaseTool::getNPCDesc(int npcID){

}

void DatabaseTool::addNPC(
		 	int npcID, 
		 	string description, 
		 	string keywords,
		 	string longdesc,
		 	string shortdesc
		 	){

}

void DatabaseTool::addZone(
		 	int zoneID,
		 	string zoneName,
		 	string description,
		 	string extendedDesc,
		 	int northID,
		 	string northDesc,
		 	string northKWs,
		 	int southID,
		 	string southDesc,
		 	string southKWs,
		 	int eastID,
		 	string eastDesc,
		 	string eastKWs,
		 	int westID,
		 	string westDesc,
		 	string westKWs
		 	){

}

string DatabaseTool::getZoneName(int zoneID){

}

string DatabaseTool::getZoneDesc(int zoneID){

}

string DatabaseTool::getzoneExtendedDesc(int zoneID){

}

int DatabaseTool::getDirectionID(int zoneID, Direction d){

}

string DatabaseTool::getDirectionDesc(int zoneID, Direction d){

}

string DatabaseTool::getDirectionKWs(int zoneID, Direction d){
	
}


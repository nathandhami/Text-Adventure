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
const int NO_BYTE_LIMIT = -1;


string DatabaseTool::quotesql( const string& s ) {
    return string("'") + s + string("'");
}

void DatabaseTool::addUser(string userID, string password) {
	Database db( DB_LOCATION );

	if (!db.Connected())
	{
		throw runtime_error("could not open database");
	}
	Query q(db);
	string sqlStatment = "INSERT INTO users VALUES (" + quotesql(userID) + "," + quotesql(password) + ");";
	q.execute(sqlStatment.c_str());
}

void DatabaseTool::addCharacter(string name, string userID){

}

int DatabaseTool::getCharID(string userID){

}

//sets charecters last know location
void DatabaseTool::putCharInZone(int charID, int zoneID){

}

int DatabaseTool::getCharsLastKnowLocation(int charId){

}

void DatabaseTool::removeCharFromCurrentZone(int charID){

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


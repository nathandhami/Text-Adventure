#ifndef DATABASE
#define DATABASE


#include <string>
#include <stdexcept>
#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Query.h"

using namespace std;
enum Direction {NORTH, EAST, SOUTH, WEST, UP, DOWN};



class DatabaseTool{
	public:
		 static void addUser(string userID, string password);

		 static int getUserID(string userName, string password);

		 static string getPassword(int userID);

		 static void addCharacter(string name, int userID);

		 static void setCharOnline(int userID);

		 static void setCharOffline(int userID);

		 static int getCharID(int userID);

		 //sets charecters last know location
		 static void putCharInZone(int charID, int zoneID);

		 static int getCharsLocation(int charID);

		 static vector<int> getAllCharsInZone(int zoneID);

		 static void placeNpcInZone(int npcID, int zoneID);

		 static vector<int> getAllNpcsInZone(int zoneID);

		 static void removeNpcFromZone(int npcID, int zone);

		 static string getNPCDesc(int npcID);

		 static void addNPC(
		 	int npcID, 
		 	string description, 
		 	string keywords,
		 	string longdesc,
		 	string shortdesc
		 	);

		 static void addZone(
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
		 	);

		 static string getZoneName(int zoneID);

		 static string getZoneDesc(int zoneID);

		 static string getzoneExtendedDesc(int zoneID);

		 static int getDirectionID(int zoneID, Direction d);

		 static string getDirectionDesc(int zoneID, Direction d);

		 static string getDirectionKWs(int zoneID, Direction d);



	private:
		
		static string quotesql( const string& s );
		static void executeSQLInsert(string statment);
};

#endif
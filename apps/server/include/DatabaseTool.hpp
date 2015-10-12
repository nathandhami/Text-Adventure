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



class DatabaseTool{
	public:
		 static bool addUser(string userName, string password);

		 static int getUserID(string userName, string password);

		 static string getPassword(int userID);

		 static void addCharacter(string name, int userID);

		 static int getCharID(int userID);

		 static bool isCharOnline(int charID);

		 static void setCharOnline(int charID);

		 static void setCharOffline(int charID);

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

		 static string getZoneExtendedDesc(int zoneID, string keyword);

		 static int getDirectionID(int zoneID, string direction);

		 static string getDirectionDesc(int zoneID, string direction);


	private:
		
		static string quotesql( const string& s );
		static bool executeSQLInsert(string statment);
		static string parseExtendedDesc(string extendedDesc, string keyword);
};

#endif
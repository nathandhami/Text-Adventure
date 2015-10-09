#ifndef DATABASE
#define DATABASE


#include <string>
#include <stdexcept>
#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <vector>

using namespace std;

class DatabaseTool{
	public:
		 static void addUser(string userID, string password);

		 static void addCharacter(string name, string userID);

		 static int getCharID(string userID);

		 //sets charecters last know location
		 static void putCharInZone(int charID, int zoneID);

		 static int getCharsLastKnowLocation(int charId);

		 static void removeCharFromCurrentZone(int charID);

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
		 	);



	private:
		static string quotesql( const string& s );
};

#endif
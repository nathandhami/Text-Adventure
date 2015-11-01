#ifndef DATABASE
#define DATABASE



#include <string>
#include <stdexcept>
#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <vector>

using namespace std;

enum Transfer {toCharacter, toZone, toNpc, toItem};
enum Target {character, npc};

class Door{
	public:
		Door();
		Door(string description, string direction, vector<string> keywords, int goesTo)
		{
			this->description = description;
			this->direction = direction;
			this->keywords = keywords;
			this->goesTo = goesTo;
		}
		~Door()
		{
		}
		string description;
		string direction;
		vector<string> keywords;
		int goesTo;

};

class ExtendedDescription{
	public:
		ExtendedDescription();
		ExtendedDescription(string description, vector<string> keywords)
		{
			this->description = description;
			this->keywords = keywords;
		}
		~ExtendedDescription(){
		}
		string description;
		vector<string> keywords;

};

class Item{
	public:
		Item();
		Item(int itemID, string longDesc, string shortDesc, vector<ExtendedDescription> extendedDescriptions, vector<string> keywords) {
			this->itemID = itemID;
			this->longDesc = longDesc;
			this->shortDesc = shortDesc;
			this->extendedDescriptions = extendedDescriptions;
			this->keywords = keywords;
			this->instanceID = 0;
		};
		~Item(){
		};
		int itemID;
		int instanceID;
		string shortDesc;
		string longDesc;
		vector<ExtendedDescription> extendedDescriptions;
		vector<string> keywords;
};

class Attributes {
	public:
		Attributes() {
			this->id = 0;
			this->level = 0;
			this->experience = 0;
			this->health = 0;
			this->strength = 0;
			this->intelligence = 0;
			this->dexterity = 0;
			this->charisma = 0;
			this->ringSlot = 0;
			this->headSlot = 0;
			this->chestSlot = 0;
			this->greavesSlot = 0;
			this->feetSlot = 0;
			this->handSlot = 0;
			this->weponSlot = 0;
		}
		Attributes(int id, int level, int experience, int health, int strength, int intelligence, int dexterity, int charisma, int ringslot, int headSlot, int chestSlot, int greavesSlot, int feetSlot, int handSlot, int weponSlot){
			this->id = id;
			this->level = level;
			this->experience = experience;
			this->health = health;
			this->strength = strength;
			this->intelligence = intelligence;
			this->dexterity = dexterity;
			this->charisma = charisma;
			this->ringSlot = ringSlot;
			this->headSlot = headSlot;
			this->chestSlot = chestSlot;
			this->greavesSlot = greavesSlot;
			this->feetSlot = feetSlot;
			this->handSlot = handSlot;
			this->weponSlot = weponSlot;
		};
		~Attributes(){
		};
		int id;
		int level;
		int experience;
		int health;
		int strength;
		int intelligence;
		int dexterity;
		int charisma;
		int ringSlot;
		int headSlot;
		int chestSlot;
		int greavesSlot;
		int feetSlot;
		int handSlot;
		int weponSlot;
		void print() {
			cout << "id: " << this->id << endl;
			cout << "level: " << this->level << endl;
			cout << "experience: " << this->experience << endl;
			cout << "health: " << this->health << endl;
			cout << "strength: " << this->strength << endl;
			cout << "intelligence: " << this->intelligence << endl;
			cout << "dexterity: " << this->dexterity << endl;
			cout << "charisma: " << this->charisma << endl;
			cout << "ringSlot: " << this->ringSlot << endl;
			cout << "headSlot: " << this->headSlot << endl;
			cout << "chestSlot: " << this->chestSlot << endl;
			cout << "greavesSlot: " << this->greavesSlot << endl;
			cout << "feetSlot: " << this->feetSlot << endl;
			cout << "handSlot: " << this->handSlot << endl;
			cout << "weponSlot: " << this->weponSlot << endl;
		}
};

class ResetCommand{
	public:
		ResetCommand();
		ResetCommand(string action, int id, int slot, int npcLimit, int room) {
			this->action = action;
			this->id = id;
			this->slot = slot;
			this->npcLimit = npcLimit;
			this->room = room;
		}
		~ResetCommand(){
		};
		string action;
		int id;
		int slot;
		int npcLimit;
		int room;
};



class DatabaseTool{
	public:
		static bool addUser(string userName, string password);

		static int getUserID(string userName, string password);

		static int getUserAuthencationLevel(int userID);

		static bool setUserAuthencationLevel(int userID, int authencationLevel);

		static string getPassword(int userID);

		static vector<string> getCharactersNames(int userID);

		static int getCharIDFromName(string name);

		static string getCharNameFromID(int charID);

		static bool addCharacter(string name, int userID, string description);

		static int getCharID(int userID);

		static bool isCharOnline(int charID);

		static void setCharOnline(int charID, string sessionID);

		static void setCharOffline(int charID);

		static string getSessionID(int charID);

		static void putCharInZone(int charID, int zoneID);

		static int getCharsLocation(int charID);

		static vector<int> getAllOnlineCharsInZone(int zoneID);

		static bool placeNpcInZone(int npcID, int zoneID);

		static vector<int> getAllNpcsInZone(int zoneID);

		static void removeNpcFromZone(int npcID, int zone);

		static int getNpcIDFromInstanceID(int npcInstanceID);

		static string getNPCDesc(int npcID);

		static bool addNPC(
		 	int npcID, 
		 	string description, 
		 	vector<string> keywords,
		 	string longdesc,
		 	string shortdesc
		 	);

		static bool addZone(
		 	int zoneID,
		 	string zoneName,
		 	string description,
		 	vector<ExtendedDescription> extendedDescriptions,
		 	vector<Door> doors
		 	);

		static string getZoneName(int zoneID);

		static string getZoneDesc(int zoneID);

		static string getZoneExtendedDesc(int zoneID, string keyword);

		static int getDirectionID(int zoneID, string direction);

		static string getDirectionDesc(int zoneID, string direction);

		static bool addItem(Item item);

		static bool spawnItemInZone(int itemID, int zoneID);

		static bool spawnItemInNpcInv(int itemID, int zoneID);

		static bool spawnItemInCharacterInv(int itemID, int zoneID);

		static bool spawnItemInItem(int itemId, int itemInstanceID);
		 
		static bool moveItem(int instanceID, Transfer where, int toID);
	
		static bool deleteItem(int instanceID);

				static bool addResetCommand(ResetCommand command);
	//to implement
		static Attributes getAttributes(int id, Target target);

		static bool updateAttributes(Attributes attributes, Target target);

		static string look(int charID);



		static void executeCommands();

		static bool addDoorToZone(int zoneID, Door door);
	
		// TMP NEW THINGS ----------------------------------------------
	
		static int createNewZone( string zoneName, string zoneDesc );
		static bool addExtendedDescriptionToZone( int zoneID, string desc, string keywords );
	
		static bool addDoorToZone( int zoneID, string description, string direction, int pointer, string keywords );
		static string getDoorDescription( int zoneID, string direction );
		static int getZoneIDBehindDoorAt( int zoneID, string direction );
	
		static bool moveCharacterToZone( int charID, int zoneID );
	
	
	private:
		
		static string quotesql( const string& s );
		static bool executeSQLInsert(string statment);
		static string parseExtendedDesc(string extendedDesc, string keyword);
		static int parseDirectionID(string doors, string direction);
		static string parseDirectionDesc(string doors, string keyword);
		static string concatDoors(vector<Door> doors);
		static string concatExtendedDescriptions(vector<ExtendedDescription> extendedDescriptions);
		static string concatKeywords(vector<string> keywords);
};






#endif
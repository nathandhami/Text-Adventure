//============================================================================
// Name        : yamlparser.cpp
// Author      : 
// Version     :
//============================================================================
// RUN EXECUTABLE build/bin/yamltest FROM parsingRoomOJECT SOURCE FOLDER

#include <iostream>
#include <assert.h>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <regex>
#include<boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "DatabaseTool.hpp"
#include <algorithm>

// choose which world to load into database
void loadYamlCppFile(std::string fileName) {
//	 config = YAML::LoadFile(fileName);
//	 std::ofstream fout(fileName);
//	 fout << config;
}

// cleans database before a new world is loaded
void resetDatabaseWorld() {
}

// cleans up strings parsed from yaml file
std::string trimString(std::string string) {

	boost::algorithm::trim(string);
	boost::erase_all(string, "'");
// std::string newStringIsSqlReady = boost::replace_all_copy(string, "'", "\\'");
//	std::cout<< string << std::endl;

	return string;
}


struct ParseRoom {
	int zoneID;
	std::string zoneName;
	std::string description;
    vector<ExtendedDescription> extendedDescriptions;
    vector<Door> doors;
};

//Door(string description, string direction, vector<string> keywords, int goesTo)
struct ParseDoor {
	std::string description;
	std::string direction;
	vector<std::string> keywords;
	int goesTo;
};

struct ParseItem {
	// 		Item(int itemID, string longDesc, string shortDesc, 
	//    vector<ExtendedDescription> extendedDescriptions, vector<string> keywords) {
	int itemID;
	std::string longDesc;
	std::string shortDesc;
	vector<ExtendedDescription> extendedDescriptions;
	vector<std::string> keywords;
};

/*
static bool addNPC(
		 	int npcID, 
		 	string description, 
		 	vector<string> keywords,
		 	string longdesc,
		 	string shortdesc
		 	);
		 	*/
struct ParseNPC{
	int npcID;
	std::string description;
	vector<std::string> keywords;
	std::string longDesc;
	std::string shortDesc;
};

struct ParseResetCommand{
	std::string action;
	int id;
	int slot;
	int npcLimit;
	int room;
};

// int zoneID, string zoneName, string description,
//string extendedDesc, int northID, string northDesc, int southID,
//string southDesc, int eastID, string eastDesc, int westID, string westDesc,
//int upID, string upDesc, int downID, string downDesc
void addRoomsToDatabase(ParseRoom parsingRoom) {
	// DatabaseTool::addZone(parsingRoom.zoneID, parsingRoom.zoneName, parsingRoom.description, parsingRoom.extendedDesc,
	// 		parsingRoom.northID, parsingRoom.northDesc, parsingRoom.southID, parsingRoom.southDesc, parsingRoom.eastID,
	// 		parsingRoom.eastDesc, parsingRoom.westID, parsingRoom.westDesc, parsingRoom.upID, parsingRoom.upDesc, parsingRoom.downID,
	// 		parsingRoom.downDesc);
//	 DatabaseTool::addZone(parsingRoom.zoneID, parsingRoom.zoneName, parsingRoom.description, "",
//	 			 	 	 	   parsingRoom.northID,  "", parsingRoom.southID, "",
//	 			 	 	 	   parsingRoom.eastID,"", parsingRoom.westID,"", parsingRoom.upID, "", parsingRoom.downID, "");
}

// test anything in the database here
// e.g query statements
void checkDatabaseContent() {
}



void parseRooms(const YAML::Node& config) {

	if (config["ROOMS"]) {

		YAML::Node roomNodes = config["ROOMS"];
		int numOfRooms = 0;

		for (YAML::iterator it = roomNodes.begin(); it != roomNodes.end();
				++it) {

			const YAML::Node roomNode = *it;
			ParseRoom parsingRoom;

			// std::cout << "Room # " << ++numOfRooms << std::endl;
			// std::cout << "------------------------" << std::endl;

			// std::cout << "Room Description: " << std::endl;

			// if there is more than one line of description
			for (unsigned int i = 0; i < roomNode["desc"].size(); ++i) {
//				std::cout << roomNode["desc"][i].as<std::string>() << std::endl;
				parsingRoom.description += " " + roomNode["desc"][i].as<std::string>();
			}

			parsingRoom.description = trimString(parsingRoom.description);
			// std::cout << parsingRoom.description << std::endl;


			vector<Door> doors;
			ParseDoor parsingDoor;
			//Door(string description, string direction, vector<string> keywords, int goesTo)

//
//			// parsingRoomocesses each door
			for (unsigned int j = 0; j < roomNode["doors"].size(); ++j) {
				// std::cout << "Door #" << j + 1 << std::endl;
				// std::cout << "---------------" << std::endl;
				// std::cout << "Door Description: " << std::endl;
				// within each door there are further categories
				std::string doorDescription;
				for (unsigned int k = 0;k < roomNode["doors"][j]["desc"].size(); ++k)
					doorDescription += " "  + roomNode["doors"][j]["desc"][k].as<std::string>();

					doorDescription = trimString(doorDescription);



//				std::cout << roomNode["doors"][j]["desc"][k] << std::endl;
//				std::cout << "dir : " << roomNode["doors"][j]["dir"] << std::endl;
				  parsingDoor.direction = roomNode["doors"][j]["dir"].as<std::string>();

//				std::cout << "keywords: " << std::endl;
					vector<std::string> doorKeywords;
				for (unsigned int k = 0;
						k < roomNode["doors"][j]["keywords"].size(); ++k){
					// std::cout << roomNode["doors"][j]["keywords"][k];
					doorKeywords.push_back(roomNode["doors"][j]["keywords"][k].as<std::string>());
			}

			parsingDoor.keywords = doorKeywords;
			parsingDoor.goesTo = roomNode["doors"][j]["to"].as<int>();
			parsingDoor.description = doorDescription;

			Door newDoor(parsingDoor.description,
						 parsingDoor.direction,
						 parsingDoor.keywords,
						 parsingDoor.goesTo);

			parsingRoom.doors.push_back(newDoor);
//							<< std::endl;
//				std::cout << "to: " << roomNode["doors"][j]["to"] << std::endl;


				// check if door leads to north,south,east,...
				// if(roomNode["doors"][j]["dir"].as<std::string>().compare("north") == 0){
				// 	parsingRoom.northID = roomNode["doors"][j]["to"].as<int>();
				// 	parsingRoom.northDesc = doorDescription;
				// 	std::cout << parsingRoom.northDesc << std::endl;
				// 	std::cout << "DOOR ID: " << parsingRoom.southID << std::endl;
				// }

				// else if(roomNode["doors"][j]["dir"].as<std::string>().compare("south") == 0){
				// 	parsingRoom.southID = roomNode["doors"][j]["to"].as<int>();
				//     parsingRoom.southDesc = doorDescription;
				//     std::cout << parsingRoom.southDesc << std::endl;
				//     std::cout << "DOOR ID: " << parsingRoom.southID << std::endl;
				// }

				// else if(roomNode["doors"][j]["dir"].as<std::string>().compare("west") == 0){
				// 	parsingRoom.westID = roomNode["doors"][j]["to"].as<int>();
				// 	parsingRoom.westDesc = doorDescription;
				// 	std::cout << parsingRoom.westDesc << std::endl;
				// 	std::cout << "DOOR ID: " << parsingRoom.westID << std::endl;
				// }

    //            else if(roomNode["doors"][j]["dir"].as<std::string>().compare("east") == 0){
				// 	parsingRoom.eastID = roomNode["doors"][j]["to"].as<int>();
				// 	parsingRoom.eastDesc = doorDescription;
				// 	std::cout << parsingRoom.eastDesc << std::endl;
				// 	std::cout << "DOOR ID: " << parsingRoom.eastID << std::endl;

				// }
				// else if(roomNode["doors"][j]["dir"].as<std::string>().compare("up") == 0){
				// 	parsingRoom.upID = roomNode["doors"][j]["to"].as<int>();
				// 	parsingRoom.upDesc = doorDescription;
				// 	std::cout << parsingRoom.upDesc << std::endl;
				// 	  std::cout << "DOOR ID: " << parsingRoom.upID << std::endl;
				// }

				// else if(roomNode["doors"][j]["dir"].as<std::string>().compare("down") == 0){
				//    parsingRoom.downID = roomNode["doors"][j]["to"].as<int>();
				//    parsingRoom.downDesc = doorDescription;
				//    std::cout << parsingRoom.downDesc << std::endl;
				//    std::cout << "DOOR ID: " << parsingRoom.downID << std::endl;
				// }
			}
//
//			// if room has more descriptions

			for (unsigned int j = 0;
					j < roomNode["extended_descriptions"].size();
					++j) {
				// std::string extendedDescription = "edesc:\\n";
				std::string extendedDescription;


				for (unsigned int k = 0;
						k < roomNode["extended_descriptions"][j]["desc"].size();
						++k){

					extendedDescription += trimString(roomNode["extended_descriptions"][j]["desc"][k].as<std::string>()) + " ";

//					std::cout
//					<< roomNode["extended_descriptions"][j]["desc"][k]
//					<< std::endl;
				}

				// if checks
				vector<std::string> extraKeywords;
				for (unsigned int k = 0;
						k < roomNode["extended_descriptions"][j]["keywords"].size();
						++k) {

//						std::cout
//						<< roomNode["extended_descriptions"][j]["keywords"][k]
//						<< std::endl;

						// extendedDescription += trimString(roomNode["extended_descriptions"][j]["keywords"][k].as<std::string>());
					extraKeywords.push_back(trimString(roomNode["extended_descriptions"][j]["keywords"][k].as<std::string>()));
				}

				// parsingRoom.keywords = roomNode["keywords"].as<vector<std::string>>();
				ExtendedDescription extendedDesc(extendedDescription,extraKeywords);
				parsingRoom.extendedDescriptions.push_back(extendedDesc);


				// std::cout << extendedDescription << std::endl;
				// parsingRoom.extendedDesc = extendedDescription;
			}
			// std::cout << "---------------" << std::endl;
////			std::cout << "room name: " << roomNode["name"] << std::endl;
			parsingRoom.zoneName = trimString(roomNode["name"].as<std::string>());
			// std::cout << parsingRoom.zoneName << std::endl;
//			std::cout << "room id: " << roomNode["id"] << std::endl;
			parsingRoom.zoneID = roomNode["id"].as<int>();
			// std::cout << "room id: " << parsingRoom.zoneID << std::endl;

			DatabaseTool::addZone(parsingRoom.zoneID,
								  parsingRoom.zoneName,
								  parsingRoom.description,
								  parsingRoom.extendedDescriptions,
								  parsingRoom.doors);

			// Insert room into database
			// addRoomsToDatabase(parsingRoom);
		}

	}
}

void parseArea(const YAML::Node& config) {
	if (config["AREA"]) {

		std::string str;
		if (config["AREA"]["name"]) {
			str = config["AREA"]["name"].as<std::string>();
			std::cout << "Loaded world: " << str << std::endl;
			std::cout << "-------------------------------------------------" << std::endl;
		} else {
			std::cout << "area does not exist" << std::endl;
		}
	} else {
		std::cout << "area does not exist" << std::endl;
	}
}

void parseNPC(const YAML::Node& config) {

	YAML::Node npcNodes = config["NPCS"];

	int numOfNPCs = 0;
	if (config["NPCS"]) {
		
		for (YAML::iterator it = npcNodes.begin(); it != npcNodes.end(); ++it) {
			const YAML::Node npcNode = *it;
			ParseNPC parsingNPC;
			// std::cout << "NPC # " << ++numOfNPCs << std::endl;
			// std::cout << "--------------------------------------------"
					// << std::endl;
			// std::cout << "Description: " << std::endl;
			for (unsigned int i = 0; i < npcNode["description"].size(); ++i) {
				// std::cout << npcNode["description"][i].as<std::string>() << std::endl;
				parsingNPC.description += trimString(npcNode["description"][i].as<std::string>()) + " ";
			}
			// std::cout << "id: " << npcNode["id"].as<int>() << std::endl;
			parsingNPC.npcID = npcNode["id"].as<int>();


			// std::cout << "keywords: " << std::endl;

			parsingNPC.keywords = npcNode["keywords"].as<vector<std::string>>();
			// for(auto s : parsingNPC.keywords)
			// 	std::cout << s << std::endl;

			// std::cout << "longdesc :" << std::endl;
			std::string longDesc;
			for (unsigned int i = 0; i < npcNode["longdesc"].size(); ++i) {
				// std::cout << npcNode["longdesc"][i].as<std::string>() << std::endl;
				longDesc += npcNode["longdesc"][i].as<std::string>() + " ";
			}
			parsingNPC.longDesc = longDesc;

			// std::cout << "short desc: "
					// << npcNode["shortdesc"].as<std::string>() << std::endl;
					parsingNPC.shortDesc = npcNode["shortdesc"].as<std::string>();

			// std::cout << "--------------------------------------------"
					// << std::endl;



			DatabaseTool::addNPC(parsingNPC.npcID,
								 parsingNPC.description,
								 parsingNPC.keywords,
								 parsingNPC.longDesc,
								 parsingNPC.shortDesc);

		}
	} else {
		std::cout << "NPCs do not exist" << std::endl;
	}




}

int numofextra = 0;

void parseItems(const YAML::Node &config){
	YAML::Node itemNodes = config["OBJECTS"];


	if(config["OBJECTS"]){
		
		vector<std::string> parsingRoomac;
		vector<ExtendedDescription> extendedDescriptions;
        vector<string> keywords;
         // keywords.push_back("dagger");
      ExtendedDescription extendedDesc("You see a dagger of great craftsmanship.  ImparsingRoominted on the side is: Merc Industries", keywords);
      // extendedDescriptions.push_back(extendedDesc);

		for(YAML::iterator it = itemNodes.begin(); it!= itemNodes.end(); ++it){
			const YAML::Node itemNode = *it;
			ParseItem parsingItem;

			parsingItem.itemID =	itemNode["id"].as<int>();
			parsingItem.shortDesc = trimString(itemNode["shortdesc"].as<std::string>());
			for (unsigned int i = 0; i < itemNode["longdesc"].size(); ++i) {
				parsingItem.longDesc = trimString(itemNode["longdesc"][i].as<std::string>());
			}
				parsingItem.keywords = itemNode["keywords"].as<vector<std::string>>();

				//extra
				YAML::Node extraNodeForItemNode =  itemNode["extra"];
				std::string edesc;

				for(unsigned int i = 0; i < extraNodeForItemNode[0]["desc"].size(); ++i){
				   edesc += trimString(extraNodeForItemNode[0]["desc"][i].as<std::string>()) + " ";
				}

				vector<std::string> extraKeywords;

				for(unsigned int i =0; i < extraNodeForItemNode[0]["keywords"].size(); ++i){
				
					extraKeywords.push_back(extraNodeForItemNode[0]["keywords"][i].as<std::string>());
				}

				ExtendedDescription extendedDesc(edesc,extraKeywords);
				parsingItem.extendedDescriptions.push_back(extendedDesc);

			Item newItem (parsingItem.itemID,parsingItem.longDesc,parsingItem.shortDesc,
						  parsingItem.extendedDescriptions,parsingItem.keywords);
			DatabaseTool::addItem(newItem);
		}



	}
}

void parseResetCommands(YAML::Node &config){

	YAML::Node resetNodes = config["RESETS"];
	
	if(config["RESETS"]){
		for(YAML::iterator it = resetNodes.begin(); it!=resetNodes.end();++it){
			YAML::Node resetNode = *it;
			ParseResetCommand parsingResetCommand;
			//initializes only optional fields
			parsingResetCommand.slot = 0;
			parsingResetCommand.room = 0;
			parsingResetCommand.npcLimit = 0;
			
			parsingResetCommand.action = resetNode["action"].as<std::string>();
			std::cout<< "action: " << parsingResetCommand.action << std::endl;
			parsingResetCommand.id = resetNode["id"].as<int>();
				std::cout<< "id: " << parsingResetCommand.id << std::endl;
			
				if(resetNode["limit"]){
					parsingResetCommand.npcLimit = resetNode["limit"].as<int>();
					std::cout << "npc Limit: " << parsingResetCommand.npcLimit << std::endl;
				}

				if(resetNode["room"]){
				parsingResetCommand.room = resetNode["room"].as<int>();
				std::cout<< "room: " <<  parsingResetCommand.room << std::endl;
			}

			if(resetNode["slot"]){
				parsingResetCommand.slot = resetNode["slot"].as<int>();
				std::cout<< "slot: " <<  parsingResetCommand.slot << std::endl;
			}


			// is the action an npc?
			if(parsingResetCommand.action.compare("npc") == 0){
				ResetCommand resetCommand("npc",
									  parsingResetCommand.id,
									  0,
									  parsingResetCommand.npcLimit,
									  parsingResetCommand.room);
				DatabaseTool::addResetCommand(resetCommand);
			}

			// equip?
			//
			


			// MISSING COMMENT to parse for reset command
			// action 
			// comment
			// slot[optional]
			// room [optional]
			// id
			// need to add to database

			// DatabaseTool::addResetCommand(resetCommand);

		}

	}
	else{
		std::cout<<"There are no reset commands in this world" << std::endl;
	}

}

int main() {

	YAML::Node config = YAML::LoadFile(
			"apps/server/databases/loadableWorlds/midgaard.yml");

	parseArea(config);
	std::cout << "Parsing NPCS...................";
	parseNPC(config);
	std::cout << "DONE" << std::endl;
	std::cout << "Parsing ROOMS...................";
	parseRooms(config);
	std::cout << "DONE" << std::endl;
	std::cout << "Parsing Items...................";
	parseItems(config);
	std::cout << "DONE" << std::endl;
	// parseResetCommands(config);

	checkDatabaseContent();

	return 0;
}

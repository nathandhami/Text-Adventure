//============================================================================
// Name        : yamlparser.cpp
// Author      : 
// Version     :
//============================================================================
// RUN EXECUTABLE build/bin/yamltest FROM parsingZoneOJECT SOURCE FOLDER

#include <iostream>
#include "YamlParser.hpp"

// choose which world to load into database
void loadYamlCppFile(std::string fileName) {
}

// cleans database before a new world is loaded
void resetDatabaseWorld() {
}

// cleans up strings parsed from yaml file
std::string trimString(std::string string) {
	boost::algorithm::trim(string);
	boost::erase_all(string, "'");
	return string;
}

void parseAreaAndAddIntoDatabase(const YAML::Node& config) {
	if (config["AREA"]) {

		std::string str;
		if (config["AREA"]["name"]) {
			str = config["AREA"]["name"].as<std::string>();
			std::cout << "Loaded world: " << str << std::endl;
			std::cout << "------------------------------------------------\n";

		} else {
			std::cout << "area does not exist" << std::endl;
		}
	} else {
		std::cout << "area does not exist" << std::endl;
	}
}

void parseZonesAndAddIntoDatabase(const YAML::Node& config) {

	if (config["ROOMS"]) {

		YAML::Node zoneNodes = config["ROOMS"];
		int numOfRooms = 0;

		for (YAML::iterator it = zoneNodes.begin(); it != zoneNodes.end();++it){

			const YAML::Node zoneNode = *it;
			ParseZone parsingZone;

			// if there is more than one line of description
			for (unsigned int i = 0; i < zoneNode["desc"].size(); ++i) {
				parsingZone.description += " " + zoneNode["desc"][i].as<std::string>();
			}
			parsingZone.description = trimString(parsingZone.description);
	
			vector<Door> doors;
			ParseDoor parsingDoor;

			// Parse zone's each door
			for (unsigned int j = 0; j < zoneNode["doors"].size(); ++j) {
	
				// within each door there are further categories needed to be parsed
				std::string doorDescription;
				for (unsigned int k = 0;k < zoneNode["doors"][j]["desc"].size(); ++k){
					doorDescription += " "  + zoneNode["doors"][j]["desc"][k].as<std::string>();
				}

				doorDescription = trimString(doorDescription);
				vector<std::string> doorKeywords;

				for (unsigned int k = 0; k < zoneNode["doors"][j]["keywords"].size(); ++k){
					doorKeywords.push_back(zoneNode["doors"][j]["keywords"][k].as<std::string>());
				}

			
			parsingDoor.direction = zoneNode["doors"][j]["dir"].as<std::string>();
			parsingDoor.keywords = doorKeywords;
			parsingDoor.goesTo = zoneNode["doors"][j]["to"].as<int>();
			parsingDoor.description = doorDescription;

			Door newDoor(parsingDoor.description,
						 parsingDoor.direction,
						 parsingDoor.keywords,
						 parsingDoor.goesTo);

			parsingZone.doors.push_back(newDoor);

			}

			// if room has more descriptions (extra)
			for (unsigned int j = 0; j < zoneNode["extended_descriptions"].size(); ++j) {
				std::string extendedDescription;

				// parses room's extra description
				for (unsigned int k = 0; k < zoneNode["extended_descriptions"][j]["desc"].size(); ++k){
					extendedDescription += trimString(zoneNode["extended_descriptions"][j]["desc"][k].as<std::string>()) 
										+ " ";
				}

				
				vector<std::string> extraKeywords;
				// parses room's extra keywords
				for (unsigned int k = 0; k < zoneNode["extended_descriptions"][j]["keywords"].size(); ++k) {
					extraKeywords.push_back(trimString(zoneNode["extended_descriptions"][j]["keywords"][k].as<std::string>()));
				}

				ExtendedDescription extendedDesc(extendedDescription,extraKeywords);
				parsingZone.extendedDescriptions.push_back(extendedDesc);
			}
		
			parsingZone.zoneName = trimString(zoneNode["name"].as<std::string>());
			parsingZone.zoneID = zoneNode["id"].as<int>();

			DatabaseTool::addZone(parsingZone.zoneID,
								  parsingZone.zoneName,
								  parsingZone.description,
								  parsingZone.extendedDescriptions,
								  parsingZone.doors);
		}

	}
}

void parseNPCAndAddIntoDatabase(const YAML::Node& config) {

	YAML::Node npcNodes = config["NPCS"];

	int numOfNPCs = 0;
	if (config["NPCS"]) {
		
		for (YAML::iterator it = npcNodes.begin(); it != npcNodes.end(); ++it){
			const YAML::Node npcNode = *it;
			ParseNPC parsingNPC;
		
			for (unsigned int i = 0; i < npcNode["description"].size(); ++i) {
			parsingNPC.description +=
			trimString(npcNode["description"][i].as<std::string>()) + " "; 
		}
			parsingNPC.npcID = npcNode["id"].as<int>(); 
			parsingNPC.keywords = npcNode["keywords"].as<vector<std::string>>();

			std::string longDesc;
			for (unsigned int i = 0; i < npcNode["longdesc"].size(); ++i) {
				longDesc += npcNode["longdesc"][i].as<std::string>() + " ";
			}
			parsingNPC.longDesc = longDesc;
			parsingNPC.shortDesc = npcNode["shortdesc"].as<std::string>();
 
			DatabaseTool::addNpc(parsingNPC.npcID,
								 parsingNPC.description,
								 parsingNPC.keywords,
								 parsingNPC.longDesc,
								 parsingNPC.shortDesc);

		}
	} else {
		std::cout << "NPCs do not exist" << std::endl;
	}




}

void parseItemsAndAddIntoDatabase(const YAML::Node &config){
	YAML::Node itemNodes = config["OBJECTS"];

	if(config["OBJECTS"]){
	
		vector<std::string> parsingZoneac;
		std::string description;
        vector<string> keywords;

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
				std::string description;

				for(unsigned int i = 0; i < extraNodeForItemNode[0]["desc"].size(); ++i){
				   description += trimString(extraNodeForItemNode[0]["desc"][i].as<std::string>()) + " ";
				}

				parsingItem.description = description;

				vector<std::string> extraKeywords;

				for(unsigned int i =0; i < extraNodeForItemNode[0]["keywords"].size(); ++i){
					extraKeywords.push_back(extraNodeForItemNode[0]["keywords"][i].as<std::string>());
				}

		
			Item newItem (parsingItem.itemID,
						  parsingItem.longDesc,
						  parsingItem.shortDesc,
						  parsingItem.description,
						  parsingItem.keywords);

			DatabaseTool::addItem(newItem);
		}



	}
}

void parseResetCommandsAndAddIntoDatabase(YAML::Node &config){

	YAML::Node resetNodes = config["RESETS"];
	
	if(config["RESETS"]){
		for(YAML::iterator it = resetNodes.begin(); it!=resetNodes.end();++it){
			YAML::Node resetNode = *it;
			ParseResetCommand parsingResetCommand;
			//initializes only optional fields
			parsingResetCommand.slot = 0;
			parsingResetCommand.room = 0;
			parsingResetCommand.npcLimit = 0;
			parsingResetCommand.state = "";
			parsingResetCommand.container = 0;

			parsingResetCommand.action = resetNode["action"].as<std::string>();
			parsingResetCommand.id = resetNode["id"].as<int>();
			
			if(resetNode["limit"]){
				parsingResetCommand.npcLimit = resetNode["limit"].as<int>();
			}

			if(resetNode["room"]){
				parsingResetCommand.room = resetNode["room"].as<int>();
			}

			if(resetNode["slot"]){
				parsingResetCommand.slot = resetNode["slot"].as<int>();
			}

			if(resetNode["container"]){
				parsingResetCommand.container = resetNode["container"].as<int>();
			}

			if(resetNode["state"]){
				parsingResetCommand.state = resetNode["state"].as<std::string>();
			}

			// is the action an npc?
			// if(parsingResetCommand.action.compare("npc") == 0){
			// 	ResetCommand resetCommand("npc",
			// 						  parsingResetCommand.id,
			// 						  0,
			// 						  parsingResetCommand.npcLimit,
			// 						  parsingResetCommand.room);

			// 	DatabaseTool::addResetCommand(resetCommand);
			// }


			ResetCommand resetCommand(parsingResetCommand.action,
									  parsingResetCommand.id,
									  parsingResetCommand.slot,
									  parsingResetCommand.npcLimit,
									  parsingResetCommand.room,
									  parsingResetCommand.state,
									  parsingResetCommand.container);
				
			DatabaseTool::addResetCommand(resetCommand);

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

	parseAreaAndAddIntoDatabase(config);
	std::cout << "Parsing NPCS...................";
	parseNPCAndAddIntoDatabase(config);
	std::cout << "DONE" << std::endl;
	std::cout << "Parsing ROOMS...................";
	parseZonesAndAddIntoDatabase(config);
	std::cout << "DONE" << std::endl;
	std::cout << "Parsing Items...................";
	parseItemsAndAddIntoDatabase(config);
	std::cout << "DONE" << std::endl;
	std::cout << "Parsing Reset Commands...................";
	parseResetCommandsAndAddIntoDatabase(config);
	std::cout << "DONE" << std::endl;

	return 0;
}

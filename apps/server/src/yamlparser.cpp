//============================================================================
// Name        : yamlparser.cpp
// Author      : 
// Version     :
//============================================================================
// RUN EXECUTABLE build/bin/yamltest FROM PROJECT SOURCE FOLDER

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
void resetDatabaseWorld(){
}

// cleans up strings parsed from yaml file
std::string trimString(std::string string) {

	boost::algorithm::trim(string);
	boost::erase_all(string,"'");

//	std::cout<< string << std::endl;

	return string;
}


struct ParseRoom {
	int zoneID;
	std::string zoneName;
	std::string description;
	std::string extendedDesc;
	int northID;
	std::string northDesc;
	int southID;
	std::string southDesc;
	int eastID;
	std::string eastDesc;
	int westID;
	std::string westDesc;
	int upID;
	std::string upDesc;
	int downID;
	std::string downDesc;
};

// int zoneID, string zoneName, string description,
//string extendedDesc, int northID, string northDesc, int southID,
//string southDesc, int eastID, string eastDesc, int westID, string westDesc,
//int upID, string upDesc, int downID, string downDesc
void addRoomsToDatabase(ParseRoom pr){
	 DatabaseTool::addZone(pr.zoneID, pr.zoneName, pr.description, "",
			 	 	 	   pr.northID,  pr.northDesc, pr.southID, pr.southDesc,
			 	 	 	   pr.eastID,pr.eastDesc, pr.westID,pr.westDesc, pr.upID, pr.upDesc, pr.downID, pr.downDesc);
//	 DatabaseTool::addZone(pr.zoneID, pr.zoneName, pr.description, "",
//	 			 	 	 	   pr.northID,  "", pr.southID, "",
//	 			 	 	 	   pr.eastID,"", pr.westID,"", pr.upID, "", pr.downID, "");
}

// test anything in the database here
// e.g query statements
void checkDatabaseContent(){
}


void parseRooms(const YAML::Node& config) {

	if (config["ROOMS"]) {


		YAML::Node roomNodes = config["ROOMS"];
		int numOfRooms = 0;

		for (YAML::iterator it = roomNodes.begin(); it != roomNodes.end(); ++it) {

			const YAML::Node roomNode = *it;
			ParseRoom pr;


			std::cout << "Room # " << ++numOfRooms << std::endl;
			std::cout << "------------------------" << std::endl;

			std::cout << "Room Description: " << std::endl;

			// if there is more than one line of description
			for (unsigned int i = 0; i < roomNode["desc"].size(); ++i) {
//				std::cout << roomNode["desc"][i].as<std::string>() << std::endl;
				pr.description += " " +  roomNode["desc"][i].as<std::string>();
			}

			pr.description = trimString(pr.description);
			std::cout << pr.description << std::endl;

			// processes each door
			for (unsigned int j = 0; j < roomNode["doors"].size(); ++j) {
				std::cout << "Door #" << j + 1 << std::endl;
				std::cout << "---------------" << std::endl;
				std::cout << "Door Description: " << std::endl;
				// within each door there are further categories
				std::string tempStr;
				for (unsigned int k = 0;k < roomNode["doors"][j]["desc"].size(); ++k)
					tempStr += " "  + roomNode["doors"][j]["desc"][k].as<std::string>();

					tempStr = trimString(tempStr);

//					std::cout << roomNode["doors"][j]["desc"][k] << std::endl;
//				std::cout << "dir : " << roomNode["doors"][j]["dir"] << std::endl;

//				std::cout << "keywords: " << std::endl;
				for (unsigned int k = 0;
						k < roomNode["doors"][j]["keywords"].size(); ++k);
//					std::cout << roomNode["doors"][j]["keywords"][k]
//							<< std::endl;
//				std::cout << "to: " << roomNode["doors"][j]["to"] << std::endl;


				// check if door leads to north,south,east,...
				if(roomNode["doors"][j]["dir"].as<std::string>().compare("north") == 0){
					pr.northID = roomNode["doors"][j]["to"].as<int>();
					pr.northDesc = tempStr;
					std::cout << pr.northDesc << std::endl;
					std::cout << "DOOR ID: " << pr.southID << std::endl;
				}

				else if(roomNode["doors"][j]["dir"].as<std::string>().compare("south") == 0){
					pr.southID = roomNode["doors"][j]["to"].as<int>();
				    pr.southDesc = tempStr;
				    std::cout << pr.southDesc << std::endl;
				    std::cout << "DOOR ID: " << pr.southID << std::endl;
				}

				else if(roomNode["doors"][j]["dir"].as<std::string>().compare("west") == 0){
					pr.westID = roomNode["doors"][j]["to"].as<int>();
					pr.westDesc = tempStr;
					std::cout << pr.westDesc << std::endl;
					std::cout << "DOOR ID: " << pr.westID << std::endl;
				}

                else if(roomNode["doors"][j]["dir"].as<std::string>().compare("east") == 0){
					pr.eastID = roomNode["doors"][j]["to"].as<int>();
					pr.eastDesc = tempStr;
					std::cout << pr.eastDesc << std::endl;
					std::cout << "DOOR ID: " << pr.eastID << std::endl;

				}
				else if(roomNode["doors"][j]["dir"].as<std::string>().compare("up") == 0){
					pr.upID = roomNode["doors"][j]["to"].as<int>();
					pr.upDesc = tempStr;
					std::cout << pr.upDesc << std::endl;
					  std::cout << "DOOR ID: " << pr.upID << std::endl;
				}

				else if(roomNode["doors"][j]["dir"].as<std::string>().compare("down") == 0){
				   pr.downID = roomNode["doors"][j]["to"].as<int>();
				   pr.downDesc = tempStr;
				   std::cout << pr.downDesc << std::endl;
				   std::cout << "DOOR ID: " << pr.downID << std::endl;
				}
			}

			// if room has more descriptions
			for (unsigned int j = 0;
					j < roomNode["extended_descriptions"].size(); ++j) {
				for (unsigned int k = 0;
						k < roomNode["extended_descriptions"][j]["desc"].size();
						++k)
					std::cout << roomNode["extended_descriptions"][j]["desc"][k]
							<< std::endl;

				// if checks
				for (unsigned int k = 0;
						k
								< roomNode["extended_descriptions"][j]["keywords"].size();
						++k)
					std::cout
							<< roomNode["extended_descriptions"][j]["keywords"][k]
							<< std::endl;
			}
			std::cout << "---------------" << std::endl;
//			std::cout << "room name: " << roomNode["name"] << std::endl;
			pr.zoneName = roomNode["name"].as<std::string>();
			std::cout << pr.zoneName << std::endl;
//			std::cout << "room id: " << roomNode["id"] << std::endl;
			pr.zoneID = roomNode["id"].as<int>();
			std::cout << "room id: " << pr.zoneID << std::endl;


			// Insert room into database
			addRoomsToDatabase(pr);
		}

	}
}


void parseArea(const YAML::Node& config) {
	if (config["AREA"]) {

		std::string str;
		if (config["AREA"]["name"]) {
			str = config["AREA"]["name"].as<std::string>();
			std::cout << "It does exist " << str << std::endl;
		} else {
			std::cout << "It does not exist" << std::endl;
		}
	} else {
		std::cout << "area does not exist" << std::endl;
	}
}

void parseNPC(const YAML::Node& config) {

	YAML::Node npcNodes = config["NPCS"];

	int numOfNPCs = 0;
	if (config["NPCS"]) {
		std::cout << "NPC DOES EXIST" << std::endl;
		for (YAML::iterator it = npcNodes.begin(); it != npcNodes.end(); ++it) {
			const YAML::Node npcNode = *it;
			std::cout << "NPC # " << ++numOfNPCs << std::endl;
			std::cout << "--------------------------------------------"
					<< std::endl;
			std::cout << "Description: " << std::endl;
			for (unsigned int i = 0; i < npcNode["description"].size(); ++i) {
				std::cout << npcNode["description"][i] << std::endl;
			}
			std::cout << "id: " << npcNode["id"].as<int>() << std::endl;
			std::cout << "keywords: " << std::endl;
			for (unsigned int i = 0; i < npcNode["keywords"].size(); ++i) {
				std::cout << npcNode["keywords"][i] << std::endl;
			}
			std::cout << "longdesc :" << std::endl;
			for (unsigned int i = 0; i < npcNode["longdesc"].size(); ++i) {
				std::cout << npcNode["longdesc"][i] << std::endl;
			}

			std::cout << "short desc: "
					<< npcNode["shortdesc"].as<std::string>() << std::endl;

			std::cout << "--------------------------------------------"
					<< std::endl;
		}
	} else {
		std::cout << "NPC does not exist" << std::endl;
	}
}

int main() {

	YAML::Node config = YAML::LoadFile("apps/server/databases/loadableWorlds/midgaard.yml");

//	parseArea(config);
//	parseNPC(config);
	parseRooms(config);

	checkDatabaseContent();

	return 0;
}

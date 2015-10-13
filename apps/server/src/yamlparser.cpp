//============================================================================
// Name        : yamlparser.cpp
// Author      : 
// Version     :
//============================================================================
// RUN EXECUTABLE FROM PROJECT FOLDER TO TEST

#include <iostream>
#include<assert.h>
#include <string>
#include<vector>
#include <yaml-cpp/yaml.h>
#include "DatabaseTool.hpp"



void loadYamlCppFile(std::string fileName) {
//	 config = YAML::LoadFile(fileName);
//	 std::ofstream fout(fileName);
//	 fout << config;
}

// int zoneID, string zoneName, string description,
//string extendedDesc, int northID, string northDesc, int southID,
//string southDesc, int eastID, string eastDesc, int westID, string westDesc,
//int upID, string upDesc, int downID, string downDesc
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

void parseRooms(const YAML::Node& config) {

	if (config["ROOMS"]) {


		YAML::Node roomNodes = config["ROOMS"];
		int numOfRooms = 0;

		for (YAML::iterator it = roomNodes.begin(); it != roomNodes.end();
				++it) {

			const YAML::Node roomNode = *it;


			std::cout << "Room # " << ++numOfRooms << std::endl;
			std::cout << "------------------------" << std::endl;

			std::cout << "Room Description: " << std::endl;
			for (unsigned int i = 0; i < roomNode["desc"].size(); ++i) {
				std::cout << roomNode["desc"][i] << std::endl;
			}

			for (unsigned int j = 0; j < roomNode["doors"].size(); ++j) {
				std::cout << "Door #" << j + 1 << std::endl;
				std::cout << "---------------" << std::endl;
				std::cout << "Door Description: " << std::endl;
				for (unsigned int k = 0;
						k < roomNode["doors"][j]["desc"].size(); ++k)
					std::cout << roomNode["doors"][j]["desc"][k] << std::endl;
				std::cout << "dir : " << roomNode["doors"][j]["dir"] << std::endl;
				std::cout << "keywords: " << std::endl;
				for (unsigned int k = 0;
						k < roomNode["doors"][j]["keywords"].size(); ++k)
					std::cout << roomNode["doors"][j]["keywords"][k]
							<< std::endl;
				std::cout << "to: " << roomNode["doors"][j]["to"] << std::endl;
			}

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
			std::cout << "room name: " << roomNode["name"] << std::endl;
			std::cout << "room id: " << roomNode["id"] << std::endl;
		}

	}
}

// int zoneID, string zoneName, string description,
//string extendedDesc, int northID, string northDesc, int southID,
//string southDesc, int eastID, string eastDesc, int westID, string westDesc,
//int upID, string upDesc, int downID, string downDesc
void addRoomsToDatabase() {
	 DatabaseTool::addZone(1234, "testZone", "", "", 0, "", 0, "", 0, "", 0, "", 0, "", 0, "");
}

void checkDatabaseContent(){
}

std::string trimString(std::string string) {

	return string;
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
//	parseRooms(config);


	checkDatabaseContent();

	return 0;
}

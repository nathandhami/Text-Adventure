//============================================================================
// Name        : yamlparser.cpp
// Author      : 
// Version     :
//============================================================================

#include <iostream>
#include<assert.h>
#include <string>
#include<vector>
#include "yaml-cpp/yaml.h"

YAML::Node config;

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

void parseRooms(const YAML::Node config){

}

void addRoomsToDatabase(){
}

int main() {
//	loadYamlCppFile("midgaard.yml");

	YAML::Node config = YAML::LoadFile("midgaard.yml");

//	YAML::Node node = YAML::Load("id: 3000");
//	assert(node.IsMap());
//
//	YAML::Node node1 = YAML::Load("NPCS:");
//		assert(node1.IsMap());
//
//	YAML::Node node2 = YAML::Load("description:");
//				assert(node2.IsMap());

	std::string str;
	if (config["AREA"]) {
		if (config["AREA"]["name"]) {

			str = config["AREA"]["name"].as<std::string>();

			std::cout << "It does exist " << str << std::endl;
		} else {
			std::cout << "It does not exist" << std::endl;
		}

	} else {
		std::cout << "area does not exist" << std::endl;
	}
//////////////////////////////////////////////////////////////////////
//	int str2;
	YAML::Node npcNodes = config["NPCS"];

	if (config["NPCS"][0]["description"]) {

		for (YAML::iterator it2 = npcNodes.begin(); it2 != npcNodes.end();
				++it2) {
			const YAML::Node npcNode = *it2;

			for (unsigned int i = 0; i < npcNode["keywords"].size(); ++i) {
				std::cout << npcNode["keywords"][i] << std::endl;
			}
		}

//		std::cout << config["NPCS"][0]["description"][0].as<std::string>() << std::endl;
//		std::cout << config["NPCS"][0]["description"][1].as<std::string>() << std::endl;
//		std::cout << config["NPCS"][0]["description"][2].as<std::string>() << std::endl;
//		std::cout << "# of string nodes: " << config["NPCS"][0]["description"].size() << std::endl;
	}

	// NPC PARSE
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

			std::cout <<"keywords: " << std::endl;
			for (unsigned int i = 0; i < npcNode["keywords"].size(); ++i) {
				std::cout << npcNode["keywords"][i] << std::endl;
			}
			std::cout <<"longdesc :" << std::endl;
			for (unsigned int i = 0; i < npcNode["longdesc"].size(); ++i) {
				std::cout << npcNode["longdesc"][i] << std::endl;
			}
//				 for(YAML::iterator it2 = npcNode["description"]; npcNode)
//				 std::cout << "long desc: " << npcNode[2]["keywords"][0].as<std::string>() << std::endl;
			std::cout << "short desc: "
					<< npcNode["shortdesc"].as<std::string>() << std::endl;
//				str2 = config["NPCS"]["description"]["id"].as<int>();
//
//				std::cout << "It does exist " << str2 << std::endl;
//			}
//			else{
//				std::cout << "It does not exist" << std::endl;
//			}
			std::cout << "--------------------------------------------"
					<< std::endl;
		}

	} else {
		std::cout << "NPC does not exist" << std::endl;
	}

	YAML::Node root = config["NPCS"];
	YAML::Node node = root["NPCS"];

	// We now have a map node, so let's iterate through:
	for (YAML::iterator it = node.begin(); it != node.end(); ++it) {
		YAML::Node key = it->first;
		YAML::Node value = it->second;
		if (key.Type() == YAML::NodeType::Sequence) {
			// This should be true; do something here with the scalar key.
			std::cout << "It is scalar" << std::endl;
		}
		if (value.Type() == YAML::NodeType::Map) {
			// This should be true; do something here with the map.
			std::cout << "It is a map" << std::endl;
		}
	}

	YAML::Node descriptionNode = config["description"];

//	YAML::Node parseDescription = YAML::Load("")

	YAML::Node primes = YAML::Load("{-rofl,-lmao}");
	if (primes.IsSequence()) {
		std::cout << "WOOT" << std::endl;
	}
	// or:
//	for (YAML::const_iterator it=primes.begin();it!=primes.end();++it) {
//		 std::cout << "Playing at " << it->first.as<std::string>() << "\n";
//	}

	if (descriptionNode.IsMap()) {
		std::cout << "It is a map" << std::endl;
	} else if (descriptionNode.IsScalar()) {
		std::cout << "It is a scalar" << std::endl;
	} else if (descriptionNode.IsSequence()) {
		std::cout << "It is a sequence" << std::endl;
	} else {
		std::cout << "It is a string" << std::endl;

	}

	return 0;
}

//============================================================================
// Name        : yamlparser.cpp
// Author      : 
// Version     :
//============================================================================

#include <iostream>
#include<assert.h>
#include <string>
#include "yaml-cpp/yaml.h"

YAML::Node config;

void loadYamlCppFile(std::string fileName){
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



int main()
{
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
	if(config["AREA"]){
		if(config["AREA"]["name"]){

			str = config["AREA"]["name"].as<std::string>();

			std::cout << "It does exist " << str << std::endl;
		}
		else{
			std::cout << "It does not exist" << std::endl;
		}

	}
	else{
		std::cout << "area does not exist" << std::endl;
	}


   std::cout << "Here's the output YAML:\n" << std::endl;
   return 0;
}

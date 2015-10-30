#ifndef YAMLPARSER_HPP
#define YAMLPARSER_HPP


#include <iostream>
#include <assert.h>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <regex>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "DatabaseTool.hpp"
#include <algorithm>

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

class YamlParser{
	public:
	void loadYamlCppFile(std::string fileName);
	void parseArea(const YAML::Node& config);
	void parseRooms(const YAML::Node& config);
	void parseNPC(const YAML::Node& config);
	void parseItems(const YAML::Node &config);
	void parseResetCommands(YAML::Node &config);
	void resetDatabaseWorld();
	void addRoomsToDatabase(ParseRoom parsingRoom);
	void checkDatabaseContent();
	std::string trimString(std::string string);
	
	private:

};






#endif

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

struct ParseZone {
	int zoneID;
	std::string zoneName;
	std::string description;
    vector<ExtendedDescription> extendedDescriptions;
    vector<Door> doors;
};

 struct ParseDoor {
	std::string description;
	std::string direction;
	vector<std::string> keywords;
	int goesTo;
};

struct ParseItem {
	int itemID;
	std::string longDesc;
	std::string shortDesc;
	vector<ExtendedDescription> extendedDescriptions;
	vector<std::string> keywords;
};

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
	std::string state;
	int container;
};

class YamlParser{
	public:
	void loadYamlCppFile(std::string fileName);
	void parseAreaAndAddIntoDatabase(const YAML::Node& config);
	void parseZonesAndAddIntoDatabase(const YAML::Node& config);
	void parseNPCAndAddIntoDatabase(const YAML::Node& config);
	void parseItemsAndAddIntoDatabase(const YAML::Node &config);
	void parseResetCommandsAndAddIntoDatabase(YAML::Node &config);
	void resetDatabaseWorld();
	std::string trimString(std::string string);
	
	private:

};






#endif

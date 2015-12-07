#include "Spellcasting.hpp"

// --------Private variables--------
	

// --------Private functions--------

string Spellcasting::getNameFromID(int thingID, Target thingType) {
	if (thingType == Target::character) {
		return DatabaseTool::getCharNameFromID(thingID);
	}
	else if (thingType == Target::npc) {
		return DatabaseTool::getNpcName(thingID);
	}
	return "";
}

string Spellcasting::castCombatSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType) {
	if (Combat::isInCombat(caster->id)) {
		int targetID = 0;
		Target targetTarget = Target::null;
		Combat::getOpponent(caster->id, &targetID, &targetTarget);
		if (targetID != target->id) {
			return "You must finish your current combat before attacking someone else.";
		}
		else {
			Combat::queueSpell(caster->id, currentSpell);
			return "";
		}
	}
	if (Combat::isInCombat(target->id)) {
		return Spellcasting::getNameFromID(target->id, targetType) + " is currently in combat, please wait patiently until they have finished their fight before casting " + currentSpell->spellName + " on them.";
	}
	return CastSpell::immediatelyCastSpell(currentSpell, caster, target, targetType);
}

string Spellcasting::castOffensiveSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType) {
	if (target->id == 0) {
		int targetID = 0;
		Target targetTarget = Target::null;
		if (!Combat::getOpponent(caster->id, &targetID, &targetTarget)) {
			return "When you are not in combat you must specify a target using the following:\n    cast " + currentSpell->spellName + " <target name>";
		}
		*target = DatabaseTool::getAttributes(targetID, targetTarget);
		targetType = targetTarget;
	}
	return Spellcasting::castCombatSpell(currentSpell, caster, target, targetType);
}

string Spellcasting::castDefensiveSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType) {
	if (target->id == 0) {
		target = caster;
	}
	return Spellcasting::castCombatSpell(currentSpell, caster, target, targetType);
}

string Spellcasting::castUtilitySpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType) {
	if (currentSpell->spellName == "stupify") {
		if (target->id == 0) {
			return "You must specify the target you want to stupify using the following:\n    cast stupify <target name>";
		}

	}
	else if (currentSpell->spellName == "switch bodies") {
		// Body swap brings a swathe of changes to our project structure. Doable, but not safely within the time we have left.
		return "Unfortunately this spell is still a work in progress.\nWe apologise for the inconvenience.";
	}
}

// Huge function, sorry
string Spellcasting::castSpell(int casterID, string arguments) {
	deque<std::string> parsedArgument;
	boost::trim(arguments);
	boost::split(parsedArgument, arguments, boost::is_any_of(" "));
	
	Target enemyType = Target::null;
	string spellName = "";
	string enemyName = "";
	for (int parsedArgumentIndex = 0; parsedArgumentIndex < parsedArgument.size(); parsedArgumentIndex++) {
		if (spellName == "") {
			spellName = parsedArgument.at(parsedArgumentIndex);
			if (enemyName.find_first_not_of(' ') == std::string::npos) {
				spellName = "";
			}
		}
		else {
			enemyName = parsedArgument.at(parsedArgumentIndex);
			if (enemyName.find_first_not_of(' ') != std::string::npos) {
				string disambiguityCheck = enemyName;
				boost::to_lower(disambiguityCheck);
				if (disambiguityCheck == "player") {
					enemyType = Target::character;
				}
				else if (disambiguityCheck == "npc") {
					enemyType = Target::npc;
				}
				else {
					break;
				}
			}
		}
	}

	if (!DatabaseTool::knowsSpell(casterID, spellName)) {
		return "You do not know the spell " + spellName + ".";
	}
	Spell currentSpell = DatabaseTool::getSpell(spellName);
	Attributes caster = DatabaseTool::getAttributes(casterID, Target::character);
	if (caster.level < currentSpell.minLevel) {
		return "Your level is too low to cast that spell (" + currentSpell.spellName + " requires level " + std::to_string(currentSpell.minLevel) + " to cast).";
	}
	Attributes target;
	if (enemyName == "") {
		// Let the cast_____Spell functions handle this (they each handle it differently)
	}
	else if (enemyType == Target::character) {
		target = DatabaseTool::getAttributes(DatabaseTool::getCharIDFromName(enemyName), Target::character);
		if (DatabaseTool::getCharsLocation(casterID) != DatabaseTool::getCharsLocation(target.id)) {
			return enemyName + " is not in your area.";
		}
	}
	else if (enemyType == Target::npc) {
		target = DatabaseTool::getAttributes(DatabaseTool::getNpcInstanceIDFromName(enemyName, DatabaseTool::getCharsLocation(casterID)), Target::npc);
		if (target.id <= 0) {
			return "There is no " + enemyName + " in the area.";
		}
	}
	else {
		int targetPlayerID = DatabaseTool::getCharIDFromName(enemyName);
		int targetNPCID = DatabaseTool::getNpcInstanceIDFromName(enemyName, DatabaseTool::getCharsLocation(casterID));
		if (targetPlayerID > 0 && targetNPCID > 0) {
			return "Do you want to cast " + spellName + " on the player named " + enemyName + " or the NPC named " + enemyName + "?\nPlease clarify by using one of these commands\n    cast " + spellName + " player <name>\n    cast " + spellName + " npc <name>";
		}
		if (targetPlayerID > 0) {
			target = DatabaseTool::getAttributes(targetPlayerID, Target::character);
			enemyType = Target::character;
			if (DatabaseTool::getCharsLocation(casterID) != DatabaseTool::getCharsLocation(target.id)) {
				return enemyName + " is not in your area.";
			}
		}
		else if (targetNPCID > 0) {
			target = DatabaseTool::getAttributes(targetNPCID, Target::npc);
			enemyType = Target::npc;
			if (target.id <= 0) {
				return "There is no " + enemyName + " in the area.";
			}
		}
	}
	if (currentSpell.archetypeID == SpellType::offensive) {
		return Spellcasting::castOffensiveSpell(&currentSpell, &caster, &target, enemyType);
	}
	else if (currentSpell.archetypeID == SpellType::defensive) {
		return Spellcasting::castDefensiveSpell(&currentSpell, &caster, &target, enemyType);
	}
	else if (currentSpell.archetypeID == SpellType::utility) {
		return Spellcasting::castUtilitySpell(&currentSpell, &caster, &target, enemyType);
	}
	else {
		return "The spell " + spellName + " has not been invented yet.";
	}
}

// --------Public functions--------

string Spellcasting::executeCommand(int playerID, Command givenCommand) {
	string command = givenCommand.type;
	string arguments = givenCommand.data;
	cout << command << " " << playerID << " " << arguments << endl;

	if (command == "cast") {
		return Spellcasting::castSpell(playerID, arguments);
	}

	return "The command " + command + " was not recognized. Check help for a list of valid commands.";
}




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
	return immediatelyCastSpell(currentSpell, caster, target, targetType);
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
		return "Unfortunately this spell is still a work in progress.\nWe apologize for the inconvenience."
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
		return "Your level is too low to cast that spell (" + currentSpell.spellName + " requires level " + currentSpell.minLevel + " to cast).";
	}
	Attributes target;
	if (enemyName == "") {
		// Let the cast_____Spell functions handle this (they each handle it differently)
	}
	else if (enemyType == Target::character) {
		target = DatabaseTool::getAttributes(DatabaseTool::getCharIDFromName(enemyName), Target::character);
	}
	else if (enemyType == Target::npc) {
		target = DatabaseTool::getAttributes(DatabaseTool::getNpcInstanceIDFromName(enemyName, DatabaseTool::getCharsLocation(casterID)), Target::npc);
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
		}
		else if (targetNPCID > 0) {
			target = DatabaseTool::getAttributes(targetNPCID, Target::npc);
			enemyType = Target::npc;
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

int Spellcasting::rollDice(int number, int sides) {
	int value = 0;
	srand (time(NULL));
	for (int diceRoll = 0; diceRoll < number; diceRoll++) {
		value += rand() % sides + 1
	}
	return value;
}

bool Spellcasting::calculateVariable(string possibleVariable, int *value, Attributes *caster, Attributes *target) {
	if (possibleVariable == "level") {
		*value = caster->level;
		return true;
	}
	else if (possibleVariable == "hp") {
		*value = target->health;
		return true;
	}
	else {
		deque<std::string> parsedDiceRoll;
		boost::split(parsedDiceRoll, possibleVariable, boost::is_any_of("d"));
		if (parsedDiceRoll.size() > 1) {
			// Extract values from possible variables before rolling
			int valueOne = 0;
			if (!Spellcasting::calculateVariable(parsedDiceRoll.at(0), &valueOne), caster, target) {
				valueOne = boost::lexical_cast<int>(parsedDiceRoll.at(0));
			}
			int valueTwo = 0;
			if (!Spellcasting::calculateVariable(parsedDiceRoll.at(1), &valueTwo, caster, target)) {
				valueTwo = boost::lexical_cast<int>(parsedDiceRoll.at(1));
			}
			*value = Spellcasting::rollDice(valueOne, valueTwo);
			return true;
		}
	}
	return false;
}

// Gah, another huge function... and it's recursive...
// Readability and such can definitely be improved by use of helper functions, will refactor if I have the time
// Does not parse exponents or nested loops or functions
int Spellcasting::parseSpellEffectStringToNumber(string spellEffect, Attributes *caster, Attributes *target) {
	string numericEquationString = "";
	deque<std::string> parsedArgument;
	// Handle brackets
	boost::split(parsedArgument, spellEffect, boost::is_any_of("("));
	if (parsedArgument.size() > 1) {
		for (int argumentIndex = 0; argumentIndex < parsedArgument.size(); argumentIndex++) {
			deque<std::string> parsedOnBracket;
			boost::split(parsedOnBracket, parsedArgument.at(argumentIndex), boost::is_any_of(")"));
			if (parsedOnBracket.size() > 1) {
				numericEquationString += parseSpellEffectStringToNumber(parsedOnBracket.at(0), caster, target).str();
				for (int bracketIndex = 1; bracketIndex < parsedOnBracket.size(); bracketIndex++) {
					numericEquationString += parsedOnBracket.at(bracketIndex);
				}
			}
			else {
				numericEquationString += parsedOnBracket.at(0);
			}
		}
		return parseSpellEffectStringToNumber(numericEquationString, caster, target);   // spellEffect with brackets replaced by their correct numerical values
	}
	// Handle subtraction
	boost::split(parsedArgument, spellEffect, boost::is_any_of("-"));
	if (parsedArgument.size() > 1) {
		float subtractionValue = parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int subtractionIndex = 1; subtractionIndex < parsedArgument.size(); subtractionIndex++) {
			subtractionValue = subtractionValue - parseSpellEffectStringToNumber(parsedArgument.at(subtractionIndex), caster, target);
		}
		return subtractionValue; 
	}
	// Handle addition
	boost::split(parsedArgument, spellEffect, boost::is_any_of("+"));
	if (parsedArgument.size() > 1) {
		float additionValue = parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int additionIndex = 1; additionIndex < parsedArgument.size(); additionIndex++) {
			additionValue = additionValue - parseSpellEffectStringToNumber(parsedArgument.at(additionIndex), caster, target);
		}
		return additionValue; 
	}
	// Handle multiplication
	boost::split(parsedArgument, spellEffect, boost::is_any_of("*"));
	if (parsedArgument.size() > 1) {
		float multiplicationValue = parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int multiplicationIndex = 1; multiplicationIndex < parsedArgument.size(); multiplicationIndex++) {
			multiplicationValue = multiplicationValue * parseSpellEffectStringToNumber(parsedArgument.at(multiplicationIndex), caster, target);
		}
		return multiplicationValue; 
	}
	// Handle division
	boost::split(parsedArgument, spellEffect, boost::is_any_of("/"));
	if (parsedArgument.size() > 1) {
		float divisionValue = parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int divisionIndex = 1; divisionIndex < parsedArgument.size(); divisionIndex++) {
			divisionValue = divisionValue / parseSpellEffectStringToNumber(parsedArgument.at(divisionIndex), caster, target);
		}
		return (int)divisionValue;  // Cuts off decimals, health is stored as int anyway
	}
	// Handle variables
	int value = 0;
	if (Spellcasting::calculateVariable(spellEffect, &value, caster, target)) {
		return value;
	}
	// Anything left should be an int
	// No checking for invalid input, can cause problems
	return boost::lexical_cast<int>(spellEffect);
}

// --------Public functions--------

string Spellcasting::executeCommand(int playerID, Command givenCommand) {
	string command = givenCommand.type;
	string arguments = givenCommand.data;
	cout << command << " " << playerID << " " << arguments << endl;

	if (command == "cast") {
		return castSpell(playerID, arguments);
	}

	return "The command " + command + " was not recognized. Check help for a list of valid commands.";
}

bool Spellcasting::hasSufficientMana(Attributes *caster, Spell *currentSpell) {
	if (caster->mana >= currentSpell->cost) {
		return true;
	}
	return false;
}

// Unsafe, is here so it can be cast from combat
// May be reworked (with related stuff) later
string Spellcasting::immediatelyCastSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType) {
	// Handle mana stuff
	Attributes casterModifier;
	casterModifier.id = caster->id;
	casterModifier.mana = -currentSpell->cost;
	if (!Spellcasting::hasSufficientMana(caster, currentSpell)) {
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You are unable to cast " + currentSpell->spellName + " (it costs " + currentSpell->cost + " mana).");
		return "";
	}
	DatabaseTool::updateAttributes(casterModifier, Target::character);

	string effect = "";
	int healthChangeSign = 1;
	Attributes targetModifier;
	targetModifier.id = target->id;
	if (currentSpell->archetypeID == 1) {
		effect = " healed";
		healthChangeSign = 1;
	}
	else if (currentSpell->archetypeID == 2) {
		effect = " damaged";
		healthChangeSign = -1;
	}
	else if (currentSpell->archetypeID == 3) {
		// Cannot be done with any of the current utility spells
		return "A utility spell was somehow cast during combat. This is a bug, please report it.";
	}
	targetModifier.health = Spellcasting::calculateSpellEffectNumber(currentSpell->effect, caster, target) * healthChangeSign;
	DatabaseTool::updateAttributes(targetModifier, targetType);
	
	// Immediately send messages to caster and target based on fields in currentSpell

	return "";
}

int Spellcasting::calculateSpellEffectNumber(string spellEffect, Attributes *caster, Attributes *target) {
	boost::trim(spellEffect);
	Spellcasting::parseSpellEffectStringToNumber(spellEffect, caster, target);
}
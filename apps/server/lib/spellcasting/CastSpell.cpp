#include "CastSpell.hpp"

// --------Private variables--------
	

// --------Private functions--------

int CastSpell::rollDice(int number, int sides) {
	int value = 0;
	srand (time(NULL));
	for (int diceRoll = 0; diceRoll < number; diceRoll++) {
		value += rand() % sides + 1
	}
	return value;
}

bool CastSpell::calculateVariable(string possibleVariable, int *value, Attributes *caster, Attributes *target) {
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
			if (!CastSpell::calculateVariable(parsedDiceRoll.at(0), &valueOne), caster, target) {
				valueOne = boost::lexical_cast<int>(parsedDiceRoll.at(0));
			}
			int valueTwo = 0;
			if (!CastSpell::calculateVariable(parsedDiceRoll.at(1), &valueTwo, caster, target)) {
				valueTwo = boost::lexical_cast<int>(parsedDiceRoll.at(1));
			}
			*value = CastSpell::rollDice(valueOne, valueTwo);
			return true;
		}
	}
	return false;
}

// Readability and such can definitely be improved by use of helper functions, will refactor if I have the time
// Does not parse exponents or nested loops or functions
int CastSpell::parseSpellEffectStringToNumber(string spellEffect, Attributes *caster, Attributes *target) {
	string numericEquationString = "";
	deque<std::string> parsedArgument;
	// Handle brackets
	boost::split(parsedArgument, spellEffect, boost::is_any_of("("));
	if (parsedArgument.size() > 1) {
		for (int argumentIndex = 0; argumentIndex < parsedArgument.size(); argumentIndex++) {
			deque<std::string> parsedOnBracket;
			boost::split(parsedOnBracket, parsedArgument.at(argumentIndex), boost::is_any_of(")"));
			if (parsedOnBracket.size() > 1) {
				numericEquationString += CastSpell::parseSpellEffectStringToNumber(parsedOnBracket.at(0), caster, target).str();
				for (int bracketIndex = 1; bracketIndex < parsedOnBracket.size(); bracketIndex++) {
					numericEquationString += parsedOnBracket.at(bracketIndex);
				}
			}
			else {
				numericEquationString += parsedOnBracket.at(0);
			}
		}
		return CastSpell::parseSpellEffectStringToNumber(numericEquationString, caster, target);   // spellEffect with brackets replaced by their correct numerical values
	}
	// Handle subtraction
	boost::split(parsedArgument, spellEffect, boost::is_any_of("-"));
	if (parsedArgument.size() > 1) {
		float subtractionValue = CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int subtractionIndex = 1; subtractionIndex < parsedArgument.size(); subtractionIndex++) {
			subtractionValue = subtractionValue - CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(subtractionIndex), caster, target);
		}
		return subtractionValue; 
	}
	// Handle addition
	boost::split(parsedArgument, spellEffect, boost::is_any_of("+"));
	if (parsedArgument.size() > 1) {
		float additionValue = CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int additionIndex = 1; additionIndex < parsedArgument.size(); additionIndex++) {
			additionValue = additionValue - CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(additionIndex), caster, target);
		}
		return additionValue; 
	}
	// Handle multiplication
	boost::split(parsedArgument, spellEffect, boost::is_any_of("*"));
	if (parsedArgument.size() > 1) {
		float multiplicationValue = CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int multiplicationIndex = 1; multiplicationIndex < parsedArgument.size(); multiplicationIndex++) {
			multiplicationValue = multiplicationValue * CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(multiplicationIndex), caster, target);
		}
		return multiplicationValue; 
	}
	// Handle division
	boost::split(parsedArgument, spellEffect, boost::is_any_of("/"));
	if (parsedArgument.size() > 1) {
		float divisionValue = CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(0), caster, target);
		for (int divisionIndex = 1; divisionIndex < parsedArgument.size(); divisionIndex++) {
			divisionValue = divisionValue / CastSpell::parseSpellEffectStringToNumber(parsedArgument.at(divisionIndex), caster, target);
		}
		return (int)divisionValue;  // Cuts off decimals, health is stored as int anyway
	}
	// Handle variables
	int value = 0;
	if (CastSpell::calculateVariable(spellEffect, &value, caster, target)) {
		return value;
	}
	// Anything left should be an int
	// No checking for invalid input, can cause problems
	return boost::lexical_cast<int>(spellEffect);
}

string CastSpell::insertNamesIntoHitMsg(string hitMsg, string name) {
	deque<std::string> parsedArgument;
	// Handle brackets
	boost::split(parsedArgument, spellEffect, boost::is_any_of("$N"));
	string newHitMsg = "";
	newHitMsg += parsedArgument.at(0);
	for (int index = 1; index < parsedArgument.size(); index++) {
		newHitMsg += name + parsedArgument.at(index);
	}
	return newHitMsg;
}

// --------Public functions---------

bool CastSpell::hasSufficientMana(Attributes *caster, Spell *currentSpell) {
	if (caster->mana >= currentSpell->cost) {
		return true;
	}
	return false;
}

// Unsafe, is here so it can be cast from combat
// May be reworked (with related stuff) later
string CastSpell::immediatelyCastSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType) {
	// Handle mana stuff
	Attributes casterModifier;
	casterModifier.id = caster->id;
	casterModifier.mana = -currentSpell->cost;
	if (!CastSpell::hasSufficientMana(caster, currentSpell)) {
		Server::sendMessageToCharacter(playerTwoID, GameCode::COMBAT, "You are unable to cast " + currentSpell->spellName + " (it costs " + currentSpell->cost + " mana).");
		return "";
	}
	DatabaseTool::updateAttributes(casterModifier, Target::character);
	Character::updateStats(caster->id);

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
	targetModifier.health = CastSpell::calculateSpellEffectNumber(currentSpell->effect, caster, target) * healthChangeSign;
	DatabaseTool::updateAttributes(targetModifier, targetType);
	Character::updateStats(target->id);
	
	// Immediately send messages to caster and target based on fields in currentSpell
	Server::sendMessageToCharacter(caster->id, GameCode::COMBAT, CastSpell::insertNamesIntoHitMsg(currentSpell->hitChar, DatabaseTool::getCharNameFromID(target->id)));
	Server::sendMessageToCharacter(target->id, GameCode::COMBAT, currentSpell->hitVict);

	return "";
}

int CastSpell::calculateSpellEffectNumber(string spellEffect, Attributes *caster, Attributes *target) {
	boost::trim(spellEffect);
	CastSpell::parseSpellEffectStringToNumber(spellEffect, caster, target);
}
#ifndef SPELLCASTING_HPP
#define SPELLCASTING_HPP

#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "Combat.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Spellcasting {
	
	static string getNameFromID(int thingID, Target thingType);

	static string castCombatSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castOffensiveSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castDefensiveSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castUtilitySpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castSpell(int casterID, string arguments);

	static int rollDice(int number, int sides);

	static bool calculateVariable(string possibleVariable, int *value, Attributes *caster, Attributes *target);
	static int parseSpellEffectStringToNumber(string spellEffect, Attributes *caster, Attributes *target);
	static string insertNamesIntoHitMsg(string hitMsg, string name)

public:

	static string executeCommand(int, Command);

	static bool hasSufficientMana(Attributes *caster, Spell *currentSpell);

	static string immediatelyCastSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);

	static int calculateSpellEffectNumber(string spellEffect, Attributes *caster, Attributes *target);
	
};

#endif
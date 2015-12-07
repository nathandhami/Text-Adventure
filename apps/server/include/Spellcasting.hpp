#ifndef SPELLCASTING_HPP
#define SPELLCASTING_HPP

#include "DatabaseTool.hpp"
#include "Command.hpp"
#include "Combat.hpp"
#include "char/Character.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <stdlib.h>

using namespace std;

class Spellcasting {
	
	static string getNameFromID(int thingID, Target thingType);

	static string castCombatSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castOffensiveSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castDefensiveSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castUtilitySpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);
	static string castSpell(int casterID, string arguments);

public:

	static string executeCommand(int, Command);

};

#endif
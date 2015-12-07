#ifndef CAST_SPELL_HPP
#define CAST_SPELL_HPP

#include "Server.hpp"
#include "DatabaseTool.hpp"
#include "char/Character.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <stdlib.h>
#include <time.h>

using namespace std;

class CastSpell {
	
	static int rollDice(int number, int sides);

	static bool calculateVariable(string possibleVariable, int *value, Attributes *caster, Attributes *target);
	static int parseSpellEffectStringToNumber(string spellEffect, Attributes *caster, Attributes *target);
	static string insertNamesIntoHitMsg(string hitMsg, string name);

public:

	static string executeCommand(int, Command);

	static bool hasSufficientMana(Attributes *caster, Spell *currentSpell);

	static string immediatelyCastSpell(Spell *currentSpell, Attributes *caster, Attributes *target, Target targetType);

	static int calculateSpellEffectNumber(string spellEffect, Attributes *caster, Attributes *target);
	
};

#endif
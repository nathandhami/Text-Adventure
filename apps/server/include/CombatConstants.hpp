#ifndef COMBAT_CONSTANTS_HPP
#define COMBAT_CONSTANTS_HPP

#include <string>

static const int ATTACK_ACTION = 0;
static const int CAST_SPELL_ACTION = 1;
static const int RETREAT_ACTION = 2;
static const int LEAVE_ACTION = 3;

static const std::string HOW_TO_START_FIGHT = "To start a fight, use one of these commands\n    fight <name>\n    fight player <name>\n    fight npc <name>";

#endif
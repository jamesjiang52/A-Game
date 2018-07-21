#ifndef PARAMETERS_H
#define PARAMETERS_H

const int PLAYER_STARTING_HEALTH = 100;
const int MAX_ARMOR = 100;  // armor is capped at this value - any higher value carries this value instead
const float MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, 0 renders armor useless, 1 negates all damage at the max armor rating
const int MAX_ENCUMBRANCE = 100;  // player can't attack or move if carrying more than this value
const float MIN_STAGGER_CHANCE = 0.4;  // min chance of being staggered in combat from a regular stagger after losing a duel
const float MAX_STAGGER_CHANCE = 0.95;  // max chance of being staggered in combat from a regular stagger after losing a duel
const float CRITICAL_DAMAGE_MULTIPLIER = 1.5  // "significant damage" in combat compared to regular damage
const float CRITICAL_STAGGER_MULTIPLIER = 1.5  // "significant stagger" in combat compared to regular stagger

#endif

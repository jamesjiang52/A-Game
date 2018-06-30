#ifndef PARAMETERS_H
#define PARAMETERS_H

const int MAX_ARMOR = 100;  // armor is capped at this value
const float MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
const int MAX_ENCUMBRANCE = 100;  // player can't attack or move if carrying more than this value
const float MIN_STAGGER_CHANCE = 0.4;  // min chance of being staggered in combat after losing a duel
const float MAX_STAGGER_CHANCE = 0.95;  // max chance of being staggered in combat after losing a duel

#endif

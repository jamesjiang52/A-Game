#ifndef PARAMETERS_H
#define PARAMETERS_H

const double MAX_ARMOR = 100;  // armor is capped at this value
const double MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
const double MAX_ENCUMBRANCE = 100;  // player can't attack or move if carrying more than this value
const double MIN_STAGGER_CHANCE = 0.4;  // min chance of being staggered in combat after losing a duel
const double MAX_STAGGER_CHANCE = 0.95;  // max chance of being staggered in combat after losing a duel

#endif

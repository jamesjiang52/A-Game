#ifndef COMBAT_H
#define COMBAT_H

#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <ctime>
#include "Parameters.hpp"
#include "DirectionClass.hpp"
#include "InteractableObjectClass.hpp"
#include "LocationClass.hpp"
#include "GenericEnemyClass.hpp"
#include "PlayerClass.hpp"
#include "MiscObjects.hpp"
#include "Weapons.hpp"
#include "Armors.hpp"
#include "Shields.hpp"
#include "Potions.hpp"
#include "Utilities.hpp"

std::string getEnemyCombatChoice(GenericEnemy *enemy);
void playerGainHealthFromEffects(Player *player, int combatTurn);
int getPlayerStaggerModifierFromEffects(Player *player, int combatTurn);
int getEnemyStaggerModifierFromEffects(Player *player, int combatTurn);
void removeInactiveEffects(Player *player, int combatTurn);
void removeAllEffects(Player *player);
bool playerCoinToss(Player *player);
bool enemyCoinToss(GenericEnemy *enemy);
void combat(Player *player, GenericEnemy *enemy);
void printPlayerEmbellishedHealthInfo(Player *player);
void printEnemyEmbellishedHealthInfo(GenericEnemy *enemy);

#endif

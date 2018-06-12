#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <regex>
#include "InteractableObjectClass.hpp"
#include "LocationClass.hpp"
#include "GenericEnemyClass.hpp"
#include "PlayerClass.hpp"

std::string addQuotes(std::string original);
std::string stripSpaces(std::string original);
void getContinueFromPlayer();
void getUserInput(Player *player);
void combat(Player *player, GenericEnemy *enemy);
void printHelpMessage();
void printLocationInfo(Player *player);
void printInventory(Player *player);

#endif

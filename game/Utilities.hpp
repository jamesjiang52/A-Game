#ifndef UTILITIES_H
#define UTILITIES_H

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

std::string addQuotes(std::string original);
std::string stripSpaces(std::string original);
void getContinueFromPlayer();
void getUserRetryOrQuit();
void getUserInput(Player *player);
std::string getEnemyCombatChoice(GenericEnemy *enemy);
bool playerCoinToss(Player *player);
bool enemyCoinToss(GenericEnemy *enemy);
void combat(Player *player, GenericEnemy *enemy);
void printCheckpointCreated();
void printCheckpointLoaded();
void printHelpMessage();
void printLocationInfo(Player *player);
void printInventory(Player *player);
void printStats(Player *player);
void printPlayerEmbellishedHealthInfo(Player *player);

#endif

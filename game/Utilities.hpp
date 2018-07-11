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
void inputGo(Player *player, std::string playerInput);
void inputTake(Player *player, std::string playerInput);
void inputDrop(Player *player, std::string playerInput);
void inputLook(Player *player, std::string playerInput);
void inputUse(Player *player, std::string playerInput);
void inputAttack(Player *player, std::string playerInput);
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
void printEnemyEmbellishedHealthInfo(GenericEnemy *enemy);

#endif

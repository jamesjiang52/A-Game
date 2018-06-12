#ifndef SCENES_H
#define SCENES_H

#include <iostream>
#include <string>
#include "PotionClass.hpp"
#include "PlayerClass.hpp"
#include "Utilities.hpp"
#include "Journal.hpp"

Player *startGame(std::string gameName, int startingHealth);
void outsideFortress(Player *player);

#endif

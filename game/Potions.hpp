#ifndef POTIONS_H
#define POTIONS_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "PotionsClass.hpp"
#include "PlayerClass.hpp"

void loafOfBreadUseFunction(Player *player);
Potion *createLoafOfBread();
void bottleOfMeadUseFunction(Player *player);
Potion *createBottleOfMead();
void squareOfChocolateUseFunction(Player *player);
Potion *createSquareOfChocolate();
void ambrosiaUseFunction(Player *player);
Potion *createAmbrosia();
void mysteriousWhitePowderUseFunction(Player *player);
Potion *createMysteriousWhitePowder();

#endif

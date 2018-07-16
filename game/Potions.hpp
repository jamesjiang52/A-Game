#ifndef POTIONS_H
#define POTIONS_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "PotionClass.hpp"
#include "PlayerClass.hpp"

void wholeWheatBreadUseFunction(Player *player);
Potion *createWholeWheatBread();
void whiteBreadUseFunction(Player *player);
Potion *createWhiteBread();
void bottleOfMeadUseFunction(Player *player);
Potion *createBottleOfMead();
void squareOfChocolateUseFunction(Player *player);
Potion *createSquareOfChocolate();
void appleUseFunction(Player *player);
Potion *createApple();
void bananaUseFunction(Player *player);
Potion *createBanana();
void durrianUseFunction(Player *player);
Potion *createDurrian();
void lemonUseFunction(Player *player);
Potion *createLemon();
void ambrosiaUseFunction(Player *player);
Potion *createAmbrosia();
void mysteriousWhitePowderUseFunction(Player *player);
Potion *createMysteriousWhitePowder();

#endif

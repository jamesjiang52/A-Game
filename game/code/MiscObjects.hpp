#ifndef MISC_OBJECTS_H
#define MISC_OBJECTS_H

#include <iostream>
#include "LocationClass.hpp"
#include "InteractableObjectClass.hpp"
#include "MiscObjectClass.hpp"
#include "PlayerClass.hpp"
#include "ActiveEffectClass.hpp"
#include "Weapons.hpp"

void unlitLanternUseFunction(Player *player);
MiscObject *createLanternUnlit();
void lanternUseFunction(Player *player);
MiscObject *createLantern();
void theArtOfWarUseFunction(Player *player);
MiscObject *createTheArtOfWar();
void stoneUseFunction(Player *player);
MiscObject *createStone();
void rainbowStoneUseFunction(Player *player);
MiscObject *createRainbowStone();
void rainbowStoneWithoutExcaliburUseFunction(Player *player);
MiscObject *createRainbowStoneWithoutExcalibur();
void quillUseFunction(Player *player);
MiscObject *createQuill();

#endif

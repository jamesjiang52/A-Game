#ifndef SCENES_H
#define SCENES_H

#include <iostream>
#include <string>
#include "Parameters.hpp"
#include "DirectionClass.hpp"
#include "InteractableObjectClass.hpp"
#include "LocationClass.hpp"
#include "WeaponClass.hpp"
#include "ArmorClass.hpp"
#include "ShieldClass.hpp"
#include "PotionClass.hpp"
#include "GenericEnemyClass.hpp"
#include "PlayerClass.hpp"
#include "Utilities.hpp"
#include "Journal.hpp"
#include "MiscObjects.hpp"
#include "Weapons.hpp"
#include "Armors.hpp"
#include "Shields.hpp"
#include "Potions.hpp"

Player *startGame();
int outsideFortress(Player *player);

#endif

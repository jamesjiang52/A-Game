#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include "DirectionClass.hpp"
#include "InteractableObjectClass.hpp"
#include "LocationClass.hpp"
#include "WeaponClass.hpp"
#include "ArmorClass.hpp"
#include "PotionClass.hpp"
#include "GenericEnemyClass.hpp"
#include "PlayerClass.hpp"
#include "Utilities.hpp"
#include "Scenes.hpp"
#include "Journal.hpp"

int main() {
    Player *player = startGame(100);  // starting health
    outsideFortress(player);
}

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <cstdlib>
#include <ctime>
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
    Player *player = startGame("Epistle, the tale of Reinbreaker's end", 100);  // starting health
    outsideFortress(player);
}

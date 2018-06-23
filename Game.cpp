#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <cstdlib>
#include <ctime>
#include "Parameters.hpp"
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
    Player playerData = *player;
    Player *playerCopy = &playerData;  // for loading checkpoints
    
    // this is the only way I could think of to save/load checkpoints
    while (1) {
        try {
            outsideFortress(player);  // throws an exception if player dies
        } catch (int e) {  // if player dies, copy original player data and loop
            Player *temp = player;
            player = playerCopy;
            delete temp;
            continue;
        }
        break;
    }
}

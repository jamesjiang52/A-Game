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
    
    while (1) {
        try {
            outsideFortress(player);  // throws an exception if player dies
        } catch (int e) {  // if player dies and wants to retry
            Player *temp = player;
            player = playerCopy;
            delete temp;
            
            playerData = *player;
            playerCopy = &playerData;
            
            printCheckpointLoaded();
            getContinueFromPlayer();
            continue;
        } catch (char e) {  // if player dies and wants to exit
            return 0;
        }
        playerData = *player;
        playerCopy = &playerData;
        break;
    }
    
    printCheckpointCreated();
    getContinueFromPlayer();
}

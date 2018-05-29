#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "DirectionClass.hpp"
#include "LocationClass.hpp"
#include "InteractableObjectClass.hpp"
#include "WeaponClass.hpp"
#include "ArmorClass.hpp"
#include "GenericEnemyClass.hpp"
#include "PlayerClass.hpp"

Player *start();

int main() {

}

Player *start() {
    /*
    Starts the game. Returns a reference to the player.
    */
    int startingHealth = 100;
    std::string tempPlayerName;
    
    std::cout << "Welcome to <placeholder>!\nBefore we begin, tell us your name.\n";
    std::getline(std::cin, tempPlayerName);

    Player *player = new Player(tempPlayerName, startingHealth, NULL);  // null since the player does not have a location yet
    return player;
}

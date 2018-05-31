#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
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
    std::string playerInput;  // a string that'll handle all player input
    
    const int STARTING_HEALTH = 100;
    
    std::cout << "Welcome to <placeholder>!\nBefore we begin, tell us your name.\n";
    std::getline(std::cin, playerInput);

    Player *player = new Player(playerInput, STARTING_HEALTH, NULL);  // null since the player does not have a location yet
    return player;
}

void getContinueFromPlayer() {
    /*
    Function to prompt and receive for user input to continue
    */
    std::string playerInput;
    std::cout << "--------------------------------------Press ENTER to continue--------------------------------------\r";
    std::getline(std::cin, playerInput);
    
    // erases the entire line printed above
    std::cout << "                                                                                                   \n\n";
}

void printHelpMessage() {
    /*
    Called if user input is "help" (case insensitive). Displays valid user commands.
    */
    std::string validCommands[] = {"go ___", "take ___ ", "use ___", "look ___", "attack ___", "inventory"};
    std::cout << "Valid commands: \n";
    
    for (int i = 0; i < 6; i++) {
        std::cout << "    " << validCommands[i] << "\n";
    }
    
    std::cout << "\n";
}

std::string stripSpaces(std::string original) {
    /*
    Removes leading and trailing spaces from input string
    */
    return std::regex_replace(original, std::regex("^ +| +$|( ) +"), "$1");
}

void getUserInput(Player *player) {
    /*
    
    */
    std::string playerInput;
    std::getline(std::cin, playerInput);
    std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
    playerInput = stripSpaces(playerInput);
    
    if (stripSpaces(playerInput) == "help") {
        printHelpMessage();
    } else if (playerInput.substr(0, 2) == "go") {  // player wants to move in direction
        if (player->getLocation()->checkStringInDirections(stripSpaces(playerInput.substr(3)))) {  // direction is valid
            
        } else {
            
        }
    } else if (playerInput.substr(0, 4) == "take") {  // player wants to take object in current location
        if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is valid
            
        } else {
            
        }
    } else if (playerInput.substr(0, 4) == "look") {  // player wants to look at object in current location
        if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is valid
        
        } else {
            
        }
    } else if (playerInput.substr(0, 3) == "use") {  // player wants to use object in inventory
        
    } else if (stripSpaces(playerInput) == "inventory") {  // inventory
    
    } else {  // command is not valid
        
    }
}

void printStartingJournalEntry() {
    std::cout << "7:34pm\n\n";
    std::cout << "Ten years. Ten years of mindless back-breaking labour, ten years of constant under-appreciation, ten\n"
                 "years of verbal abuse, ten years of manipulation and deceit. Ten years endured, and for what?\n"
                 "Demotion and an official reprimand for \"unprofessional\" conduct. These were the thoughts that raged\n"
                 "through my mind as I approached Reinbreaker, the towering presence that loomed over the entire\n"
                 "valley, its impregnability famed across the land. Considered the sole reason why the west had\n"
                 "remained relatively unmarred after the many years of war, the fortress stood proudly along the\n"
                 "river, shining as a beacon of imperial might. Tonight, this will change, its illusion of power\n"
                 "destroyed, the mighty empire brought to its feet, and I shall be the cause. My name once revered as\n"
                 "the next imperator will now be spit upon alongside the dregs and scum, but vengeance must have its\n"
                 "due.\n\n";
}

void outsideFortress(Player *player) {
    printStartingJournalEntry();
    getContinueFromPlayer();
    
    Location *outsideFortressWalls_1 = new Location("Outside Fortress Walls", "<placeholder description>");
    Location *outsideFortressWalls_2 = new Location("Outside Fortress Walls", "<placeholder description>");
    Location *outsideFortressWalls_3 = new Location("Outside Fortress Walls", "<placeholder description>");  // something interesting probably happens here
    
    player->setLocation(outsideFortressWalls_1);
    player->printLocationInfo();
}

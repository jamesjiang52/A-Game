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

Player *start();
void getContinueFromPlayer();
void printHelpMessage();
std::string addQuotes(std::string original);
std::string stripSpaces(std::string original);
void getUserInput(Player *player);
void printStartingJournalEntry();
void outsideFortress(Player *player);

int main() {
    Player *player = start();
    outsideFortress(player);
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
    std::cout << "--------------------------------------Press ENTER to continue--------------------------------------";
    std::getline(std::cin, playerInput);
    
    // erases the entire line printed above
    std::cout << "\033[A\r\33[2K\n\n";
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

std::string addQuotes(std::string original) {
    /*
    Adds encompassing quotes to input string
    */
    return "\"" + original + "\"";
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
    std::cout << "What do I do?\n";
    std::getline(std::cin, playerInput);
    std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
    playerInput = stripSpaces(playerInput);
    
    if (stripSpaces(playerInput) == "help") {
        printHelpMessage();
    } else if (playerInput.substr(0, 2) == "go") {  // player wants to move in direction
        if (player->getLocation()->checkStringInDirections(stripSpaces(playerInput.substr(3)))) {  // direction is valid
            player->setLocation(player->getLocation()->getDirectionFromString(stripSpaces(playerInput.substr(3)))->getDestination());
            player->printLocationInfo();
        } else {
            std::cout << addQuotes(stripSpaces(playerInput.substr(3))) << " does not look like a direction I can travel in.\n\n";
        }
    } else if (playerInput.substr(0, 4) == "take") {  // player wants to take object
        if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is valid
            InteractableObject *object = player->getLocation()->getObjectFromString(stripSpaces(playerInput.substr(5)));
            player->addToInventory(object);
            player->getLocation()->removeInteractableObject(object);
            std::cout << "I pick up the " << stripSpaces(playerInput.substr(5)) << " and put it in my knapsack.\n\n";
        } else {
            std::cout << "I do not see " << addQuotes(stripSpaces(playerInput.substr(5))) << ".\n\n";
        }
    } else if (playerInput.substr(0, 4) == "look") {  // player wants to look at object
        if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is in current location
            std::cout << player->getLocation()->getObjectFromString(stripSpaces(playerInput.substr(5)))->getDescription();
        } else if (player->checkStringInInventory(stripSpaces(playerInput.substr(5)))) {  // object is in inventory
            std::cout << player->getObjectFromString(stripSpaces(playerInput.substr(5)))->getDescription();
        } else {
            std::cout << "I do not see " << addQuotes(stripSpaces(playerInput.substr(5))) << " around me, nor can I find it in my knapsack.\n\n";
        }
    } else if (playerInput.substr(0, 3) == "use") {  // player wants to use object
        if (player->checkStringInInventory(stripSpaces(playerInput.substr(4)))) {  // object is in inventory
            InteractableObject *object = player->getObjectFromString(stripSpaces(playerInput.substr(4)));
            object->use(player);
            // NOTE TO SELF: REMEMBER TO PUT THE OBJECT USE MESSAGE IN ITS use() FUNCTION
        } else {
            std::cout << "I cannot find " << addQuotes(stripSpaces(playerInput.substr(4))) << " in my knapsack.\n\n";
        }
    } else if (playerInput.substr(0, 6) == "attack") {  // player wants to attack
        // I'll ignore this for now since I haven't thought about how to do combat yet
        ;
    } else if (playerInput == "inventory") {  // inventory
        player->printInventory();
    } else {  // command is not valid
        std::cout << "I do not know that action.\n\n";
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
                 "destroyed, the mighty empire brought to its knees, and I shall be the cause. My name once revered as\n"
                 "the next imperator will now be spit upon alongside the dregs and scum, but vengeance must have its\n"
                 "due.\n\n";
}

void outsideFortress(Player *player) {
    printStartingJournalEntry();
    getContinueFromPlayer();
    
    InteractableObject *ironSword = new Weapon("iron sword", "", 10);  // name, description, damage
    InteractableObject *clothes = new Armor("street clothes", "", 0);  // name, description, armor
    InteractableObject *bread = new Potion("loaf of bread", "", 5);  // name, description, healAmount
    InteractableObject *meadBottle = new Potion("bottle of mead", "", 8);
    InteractableObject *lantern = new InteractableObject("lantern", "",);  // name, description
    
    player->addToInventory(ironSword);
    player->addToInventory(clothes);
    player->addToInventory(bread);
    player->addToInventory(meadBottle);
    player->addToInventory(lantern);
    
    Location *outsideFortressWalls = new Location(
        "Outside Fortress Walls", 
        "I am on a narrow cobblestoned trail. Just up ahead to the north is the stronghold, a mere minute's\n"
        "walk away, its imperial towers and walls standing unshaken against the cannons of war unending. Just\n"
        "off the path and down the hill, the river roars its great presence, mighty and boisterous. To my\n"
        "south, I can only see the unbroken plains of grass ripple and roll as the evening gale sings its\n"
        "serene nocturne against the fading light. The fortress calls me closer.\n"
    );

    Location *outsideFortressWalls_ = new Location(  // if player chooses to go south
        "Outside Fortress Walls",
        "I turn around and try to leave, but the stronghold beckons me closer. I cannot leave until the\n"
        "fortress is mine.\n"
    );

    Location *riverShore = new Location(
        "River Shore",
        "I veer off the path and put my foot into the water. It is freezing cold. I do not dare go into the\n"
        "water with just my streets clothes, lest I lose my strength and become incapable of pulling myself\n"
        "back to the safety of shore. I need to get back onto the path.\n"
    );

    Location *fortressGate = new Location(
        "Fortress Gate",
        "I approach the first of the fortress' triple walls. I am greeted by a closed gate as I am commanded\n"
        "to reveal what I carry. A standard issue iron sword, a lantern, a loaf of bread, a bottle of mead,\n"
        "and the clothes on my back. Assent is given as the bridge is lowered and the gate is raised. Fools.\n"
    );

    Direction *outsideFortressWallsNorth = new Direction("north", fortressGate);
    Direction *outsideFortressWallsSouth = new Direction("south", outsideFortressWalls_);
    Direction *outsideFortressWallsRiver = new Direction("river", riverShore);
    Direction *riverShoreBack = new Direction("back", outsideFortressWalls);
    Direction *riverShorePath = new Direction("path", outsideFortressWalls);

    outsideFortressWalls->addDirection(outsideFortressWallsNorth);
    outsideFortressWalls->addDirection(outsideFortressWallsSouth);
    outsideFortressWalls->addDirection(outsideFortressWallsRiver);
    outsideFortressWalls_->addDirection(outsideFortressWallsNorth);
    outsideFortressWalls_->addDirection(outsideFortressWallsRiver);
    riverShore->addDirection(riverShoreBack);
    riverShore->addDirection(riverShorePath);

    player->setLocation(outsideFortressWalls);
    player->printLocationInfo();

    while (1) {
        getUserInput(player);
    }
}

#include "Scenes.hpp"

Player *startGame(int startingHealth) {
    /*
    Starts the game. Returns a reference to the player.
    */
    std::string playerInput;
    
    std::cout << "Welcome to <placeholder>!\nBefore we begin, tell us your name.\n";
    std::getline(std::cin, playerInput);

    Player *player = new Player(playerInput, startingHealth, NULL);  // null since the player does not have a location yet
    return player;
}

void outsideFortress(Player *player) {
    printOutsideFortressJournalEntry();
    getContinueFromPlayer();
    
    InteractableObject *ironSword = new Weapon("iron sword", "", 10);  // name, description, damage
    InteractableObject *clothes = new Armor("street clothes", "", 0);  // name, description, armor
    InteractableObject *bread = new Potion("loaf of bread", "", 5);  // name, description, healAmount
    InteractableObject *meadBottle = new Potion("bottle of mead", "", 8);
    InteractableObject *lantern = new InteractableObject("lantern", "");  // name, description
    
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
    printLocationInfo(player);

    while (1) {
        getUserInput(player);
    }
}

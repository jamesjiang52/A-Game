#include "Scenes.hpp"

Player *startGame(std::string gameName, int startingHealth) {
    /*
    Starts the game. Returns a reference to the player.
    */
    std::string playerInput;
    
    std::cout << "Welcome to " << gameName << "!\nBefore we begin, tell us your name.\n";
    std::getline(std::cin, playerInput);
    std::cout << "\n\n";

    Player *player = new Player(playerInput, startingHealth);
    return player;
}

void outsideFortress(Player *player) {
    // set-up for scene
    printOutsideFortressJournalEntry();
    getContinueFromPlayer();
    
    // create objects and add to player's inventory
    InteractableObject *ironSword = new Weapon("iron sword", "", 10);  // name, description, damage
    InteractableObject *clothes = new Armor("street clothes", "", 0);  // name, description, armor
    InteractableObject *bread = new Potion("loaf of bread", "", 5);  // name, description, healAmount
    InteractableObject *meadBottle = new Potion("bottle of mead", "", 8);
    InteractableObject *lantern = new InteractableObject("lantern", "");  // name, description

    lantern->setUseMessage("I light the lantern. The flame bathes me in an ember glow.");
    
    player->addToInventory(ironSword);
    player->addToInventory(clothes);
    player->addToInventory(bread);
    player->addToInventory(meadBottle);
    player->addToInventory(lantern);
    
    // create locations
    Location *outsideFortressWalls = new Location(
        "Outside Fortress Walls", 
        "I am on a narrow cobblestoned trail. Just up ahead to the north is the stronghold, a mere minute's\n"
        "walk away, its imperial towers and walls standing unshaken against the cannons of war unending. Just\n"
        "off the path and down the hill, the river roars its great presence, mighty and boisterous. To the\n"
        "south, I can only see the unbroken plains of grass ripple and roll as the evening gale sings its\n"
        "serene nocturne against the fading light. The fortress calls me closer.\n"
    );
    
    Location *outsideFortressWallsCloser = new Location(
        "Outside Fortress Walls",
        "I walk north along the beaten path, closer to the fortress. The sun's dusk paints the sky a grand\n"
        "spectacle of amber and violet. I can see not a single cloud, not even a flurry. The fortress calls\n"
        "me closer.\n"

    Location *outsideFortressWallsAway = new Location(  // if player chooses to go south
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

    // create directions (that player can use to travel to certain locations)
    Direction *outsideFortressWallsNorth = new Direction("north", outsideFortressWallsCloser);
    Direction *outsideFortressWallsFortress = new Direction("fortress", outsideFortressWallsCloser);  // treated same as direction on previous line
    Direction *outsideFortressWallsSouth = new Direction("south", outsideFortressWallsAway);
    Direction *outsideFortressWallsDummy = new Direction("south", outsideFortressWallsAway);  // if player chooses to go south twice
    Direction *outsideFortressWallsRiver = new Direction("river", riverShore);
    Direction *riverShoreBack = new Direction("back", outsideFortressWalls);
    Direction *riverShorePath = new Direction("path", outsideFortressWalls);  // treated same as direction on previous line
    Direction *outsideFortressWallsCloserNorth = new Direction("north", fortressGate);
    Direction *outsideFortressWallsCloserFortress = new Direction("fortress", fortressGate);  // treated same as direction on previous line
    Direction *outsideFortressWallsCloserSouth = new Direction("south", outsideFortressWalls);

    // add directions to locations
    outsideFortressWalls->addDirection(outsideFortressWallsNorth);
    outsideFortressWalls->addDirection(outsideFortressWallsFortress);
    outsideFortressWalls->addDirection(outsideFortressWallsRiver);
    outsideFortressWalls->addDirection(outsideFortressWallsSouth);
    
    outsideFortressWallsAway->addDirection(outsideFortressWallsNorth);
    outsideFortressWallsAway->addDirection(outsideFortressWallsFortress);
    outsideFortressWallsAway->addDirection(outsideFortressWallsRiver);
    outsideFortressWallsAway->addDirection(outsideFortressWallsDummy);
    
    outsideFortressWallsCloser->addDirection(outsideFortressWallsCloserNorth);
    outsideFortressWallsCloser->addDirection(outsideFortressWallsCloserFortress);
    outsideFortressWallsCloser->addDirection(outsideFortressWallsCloserSouth);
    outsideFortressWallsCloser->addDirection(outsideFortressWallsCloserNorth);
    
    riverShore->addDirection(riverShoreBack);
    riverShore->addDirection(riverShorePath);

    // play scene
    player->setLocation(outsideFortressWalls);
    printLocationInfo(player);

    while (player->getLocation != fortressGate) {
        getUserInput(player);
    }
    
    // functions terminates when player reaches the fortress gate (ready for next scene)
}

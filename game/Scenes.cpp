#include "Scenes.hpp"

Player *startGame() {
    /*
    Starts the game. Returns a reference to the player.
    */
    std::string playerInput;
    
    std::cout << "What is my name?\n";
    std::getline(std::cin, playerInput);
    std::cout << "\n\n";

    getContinueFromPlayer();
    Player *player = new Player(playerInput, PLAYER_STARTING_HEALTH);
    return player;
}

void outsideFortress(Player *player) {
    // set-up for scene
    printOutsideFortressJournalEntry();
    getContinueFromPlayer();
    
    // create objects and add to player's inventory
    Weapon *bronzeSword = createBronzeSword();
    Armor *streetClothes = createStreetClothes();
    Potion *whiteBread = createWhiteBread();
    Potion *meadBottle = createBottleOfMead();
    InteractableObject *lantern = createLanternUnlit();
    
    player->addToInventory(bronzeSword);
    player->addToInventory(streetClothes);
    player->addToInventory(whiteBread);
    player->addToInventory(meadBottle);
    player->addToInventory(lantern);
    
    player->setWeapon(bronzeSword);
    player->setArmor(streetClothes);
    
    // create locations
    Location *outsideFortressWalls = new Location(
        "Outside Fortress Walls", 
        "I am on a narrow cobblestoned trail. Just up ahead to the north is the stronghold, a mere minute's "
        "walk away, its imperial towers and walls standing unshaken against the cannons of war unending. Just "
        "off the path and down the hill, the river roars its great presence, mighty and boisterous. To the "
        "south, I can only see the unbroken plains of grass ripple and roll as the evening gale sings its "
        "serene nocturne against the fading light. The fortress calls me closer.\n"
    );
    
    Location *outsideFortressWallsCloser = new Location(
        "Outside Fortress Walls",
        "I walk north along the beaten path, closer to the fortress. The sun's dusk paints the sky a grand "
        "spectacle of amber and violet. I can see not a single cloud, not even a flurry. The fortress calls "
        "me closer.\n"
    );

    Location *outsideFortressWallsAway = new Location(  // if player chooses to go south
        "Outside Fortress Walls",
        "I turn around and try to leave, but the stronghold beckons me closer. I cannot leave until the "
        "fortress is mine.\n"
    );

    Location *riverShore = new Location(
        "River Shore",
        "I veer off the path and dip my foot into the water. It is freezing cold. I do not dare go into the "
        "water with just my streets clothes, lest I lose my strength and become incapable of pulling myself "
        "back to the safety of shore. I need to get back onto the path.\n"
    );

    Location *fortressGate = new Location(
        "Fortress Gate",
        "I approach the first of the fortress' triple walls. I am greeted by a closed gate as I am commanded "
        "to reveal what I carry. A stolen bronze sword, a lantern, a loaf of bread, a bottle of mead, "
        "and the clothes on my back. Assent is given as the bridge is lowered and the gate is raised. Fools. "
        "This is the beginning of the end.\n"
    );

    // create directions (that player can use to travel to certain locations)
    Direction *outsideFortressWallsNorth = new Direction("north", outsideFortressWallsCloser);
    Direction *outsideFortressWallsSouth = new Direction("south", outsideFortressWallsAway);
    Direction *outsideFortressWallsRiver = new Direction("river", riverShore);
    Direction *riverShorePath = new Direction("path", outsideFortressWalls);
    Direction *outsideFortressWallsCloserNorth = new Direction("north", fortressGate);
    Direction *outsideFortressWallsCloserSouth = new Direction("south", outsideFortressWalls);

    // add directions to locations
    outsideFortressWalls->addDirection(outsideFortressWallsNorth);
    outsideFortressWalls->addDirection(outsideFortressWallsRiver);
    outsideFortressWalls->addDirection(outsideFortressWallsSouth);
    
    outsideFortressWallsAway->addDirection(outsideFortressWallsNorth);
    outsideFortressWallsAway->addDirection(outsideFortressWallsRiver);
    outsideFortressWallsAway->addDirection(outsideFortressWallsSouth);
    
    outsideFortressWallsCloser->addDirection(outsideFortressWallsCloserNorth);
    outsideFortressWallsCloser->addDirection(outsideFortressWallsCloserSouth);
    
    riverShore->addDirection(riverShorePath);
    
    // test enemy (will remove later)
    Weapon *stoneSword = createStoneSword();
    GenericEnemy *guard = new GenericEnemy("castle guard", 20);
    guard->setWeapon(stoneSword);
    guard->setArmor(streetClothes);
    
    outsideFortressWallsCloser->addEnemy(guard);

    // play scene
    player->setLocation(outsideFortressWalls);
    printLocationInfo(player);

    while (player->getLocation() != fortressGate) {
        getUserInput(player);
    }
    // function terminates when player reaches fortress gate
}

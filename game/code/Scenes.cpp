#include "Scenes.hpp"

Player *startGame() {
    /*
    Starts the game. Returns a reference to the player.
    */
    std::string playerInput;
    
    std::cout << "What is my name?\n";
    std::getline(std::cin, playerInput);
    std::cout << "\nAt any time, use \"help\" to display a list of available commands.\n\n";

    printCheckpointCreated();
    getContinueFromPlayer();
    Player *player = new Player(correctCase(playerInput), PLAYER_STARTING_HEALTH);
    return player;
}

int outsideFortress(Player *player) {
    // set up scene
    printOutsideFortressJournalEntry();
    getContinueFromPlayer();
    
    // since this is the first scene, initialize player inventory
    Weapon *bronzeSword = createBronzeSword();
    Weapon *stoneSword = createStoneSword();
    Armor *streetClothes = createStreetClothes();
    Potion *whiteBread = createWhiteBread();
    Potion *meadBottle = createBottleOfMead();
    MiscObject *lantern = createLanternUnlit();
    
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
        "serene nocturne against the fading light."
    );
    
    Location *fortressWalls = new Location(
        "By the Fortress Walls",
        "I walk north along the beaten path, closer to the fortress. The sun's dusk paints the sky a grand "
        "spectacle of amber and violet. I can see not a single cloud, not even a flurry."
    );

    Location *outsideFortressWallsAway = new Location(  // if player chooses to go south
        "Outside Fortress Walls",
        "I turn around and try to leave, but I must remember my purpose. I cannot leave until the "
        "fortress is mine."
    );

    Location *riverShore = new Location(
        "River Shore",
        "I veer off the path and dip my foot into the water. It is freezing cold. I do not dare go into the "
        "water, lest I lose my strength and become incapable of pulling myself "
        "back to the safety of shore. I should get back onto the path."
    );

    Location *moat = new Location(
        "The Moat",
        "I approach the first of the fortress' triple walls. I am greeted by a deep ditch and a raised bridge. I look "
        "down to see a rush of inky black water at least two meters deep. Across stands two watchmen that guard the gate, "
        "one of whom greets me and commands me to reveal what I carry. A bronze sword, a lantern, a loaf of bread, "
        "a bottle of mead, and the clothes on my back. Assent is given as the bridge is lowered and the gate is raised."
    );
    
    Location *gate = new Location(
        "The Gate",
        "I step across the wooden bridge, careful to mind my step and not trip. As I step off the other side, a curious stone "
        "catches my interest, impossibly smooth and round. Onwards, I am greeted by the two watchmen."
    );

    // create directions
    Direction *outsideFortressWallsNorth = new Direction("north", fortressWalls);
    Direction *outsideFortressWallsSouth = new Direction("south", outsideFortressWallsAway);
    Direction *outsideFortressWallsRiver = new Direction("river", riverShore);
    Direction *riverShorePath = new Direction("path", outsideFortressWalls);
    Direction *fortressWallsNorth = new Direction("north", moat);
    Direction *fortressWallsSouth = new Direction("south", outsideFortressWalls);
    Direction *moatGate = new Direction("gate", gate);

    // add directions to locations
    outsideFortressWalls->addDirection(outsideFortressWallsNorth);
    outsideFortressWalls->addDirection(outsideFortressWallsRiver);
    outsideFortressWalls->addDirection(outsideFortressWallsSouth);
    
    outsideFortressWallsAway->addDirection(outsideFortressWallsNorth);
    outsideFortressWallsAway->addDirection(outsideFortressWallsRiver);
    outsideFortressWallsAway->addDirection(outsideFortressWallsSouth);
    
    fortressWalls->addDirection(fortressWallsNorth);
    fortressWalls->addDirection(fortressWallsSouth);
    
    riverShore->addDirection(riverShorePath);
    
    moat->addDirection(moatGate);

    // add things to locations
    InteractableObject *stone = createStone();
    
    GenericEnemy *guard1 = new GenericEnemy("guard", 20);
    GenericEnemy *guard2 = new GenericEnemy("guard", 20);
    guard1->setWeapon(stoneSword);
    guard1->setArmor(streetClothes);
    guard1->addToInventory(stoneSword);
    guard1->addToInventory(streetClothes);
    
    guard2->setWeapon(stoneSword);
    guard2->setArmor(streetClothes);
    guard2->addToInventory(stoneSword);
    guard2->addToInventory(streetClothes);
    
    gate->addEnemy(guard1);
    gate->addEnemy(guard2);
    gate->addInteractableObject(stone);

    // play scene
    player->setLocation(outsideFortressWalls);
    printLocationInfo(player);

    try {
        while (1) {
            getUserInput(player);
        }
    } catch (int e) {
        return -1;
    }
    return 0;
}

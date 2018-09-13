#include "Utilities.hpp"

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

std::string correctCase(std::string original) {
    std::string newStr = original;
    newStr[0] = std::toupper(original[0]);
    for (size_t i = 1; i < original.length(); i++) {
        newStr[i] = std::tolower(original[i]);
    }
    return newStr;
}

void getContinueFromPlayer() {
    /*
    Function to prompt and receive for user input to continue
    */
    std::string playerInput;
    std::cout << "-------------------------------------- Press ENTER to continue --------------------------------------";
    std::getline(std::cin, playerInput);
    std::cout << "\n";
}

void getUserRetryOrQuit() {
    std::string playerInput;
    std::cout << "You have died. Type \"retry\" to load from the last checkpoint or \"quit\" to exit the game.\n";
    do {
        std::getline(std::cin, playerInput);
        std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
        playerInput = stripSpaces(playerInput);
        if (playerInput == "retry") {
            std::cout << "\n";
            throw 10;  // another function catches this integer exception
        } else if (playerInput == "quit") {
            std::cout << "\n";
            throw 'e';  // another function catches this char exception
        } else {
            std::cout << "\n\n";
        }
    } while ((playerInput != "retry") && (playerInput != "quit"));
}

void getUserInput(Player *player) {
    /*
    Gets and performs the player action
    */
    std::string playerInput;
    std::cout << "What do I do?\n";
    std::getline(std::cin, playerInput);
    std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
    playerInput = stripSpaces(playerInput);
    
    if (playerInput == "") {
        std::cout << "\n";
    } else if (playerInput == "die") {  // testing
        getUserRetryOrQuit();
    } else if (playerInput == "help") {
        printHelpMessage();
    } else if (playerInput.substr(0, 2) == "go") {  // player wants to move in direction
        inputGo(player, playerInput);
    } else if (playerInput.substr(0, 4) == "take") {  // player wants to take object
        inputTake(player, playerInput);
    } else if (playerInput.substr(0, 4) == "drop") {  // player wants to drop object
        inputDrop(player, playerInput);
    } else if (playerInput.substr(0, 4) == "look") {  // player wants to look at object
        inputLook(player, playerInput);
    } else if (playerInput.substr(0, 3) == "use") {  // player wants to use object
        inputUse(player, playerInput);
    } else if (playerInput.substr(0, 6) == "attack") {  // player wants to attack
        inputAttack(player, playerInput);
    } else if (playerInput == "inventory") {  // inventory
        printInventory(player);
    } else if (playerInput == "stats") {
        printStats(player);
    } else if (playerInput == "journal") {
        journal(player->getQuestStage(), player->getName());
        printLocationInfo(player);
    } else {  // command is not valid
        std::cout << "I do not know how to " << playerInput <<".\n\n";
    }
}

void inputGo(Player *player, std::string playerInput) {
    if (player->isOverEncumbered()) {
        std::cout << "I am carrying too much. I need to drop something before I can move.\n\n";
    } else if (playerInput.length() > 3) {
        if (player->getLocation()->checkStringInDirections(stripSpaces(playerInput.substr(3)))) {  // direction is valid
            player->setLocation(player->getLocation()->getDirectionFromString(stripSpaces(playerInput.substr(3)))->getDestination());
            printLocationInfo(player);
        } else {
            std::cout << addQuotes(stripSpaces(playerInput.substr(3))) << " does not look like a direction I can travel in.\n\n";
        }
    } else {
        std::cout << "\n";
    }
}

void inputTake(Player *player, std::string playerInput) {
    if (player->isOverEncumbered()) {
        std::cout << "I am carrying too much. I need to drop something before I pick up anything else.\n\n";
    } else if (playerInput.length() > 5) {
        if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is valid
            InteractableObject *object = player->getLocation()->getObjectFromString(stripSpaces(playerInput.substr(5)));
            player->addToInventory(object);
            player->getLocation()->removeInteractableObject(object);
            std::cout << "I pick up the " << stripSpaces(playerInput.substr(5)) << " and put it in my knapsack (+" << object->getEncumbrance() << " encumbrance).\n\n";
        } else {
            std::cout << "I do not see " << addQuotes(stripSpaces(playerInput.substr(5))) << ".\n\n";
        }
    } else {
        std::cout << "\n";
    }
}

void inputDrop(Player *player, std::string playerInput) {
    if (playerInput.length() > 5) {
        if (player->checkStringInInventory(stripSpaces(playerInput.substr(5)))) {  // object is in inventory
            InteractableObject *object = player->getObjectFromString(stripSpaces(playerInput.substr(4)));
            if (object == player->getWeapon()) {  // don't let player drop their equipped weapon
                std::vector<InteractableObject*> inventory = player->getInventory();
                if (std::count(inventory.begin(), inventory.end(), object) > 1) {  // but their equipped weapon is duplicate
                    player->removeFromInventory(object);
                    player->getLocation()->addInteractableObject(object);
                    std::cout << "I open my knapsack and grab the " << stripSpaces(playerInput.substr(5)) << ", dropping it on the ground beside me (-" << object->getEncumbrance() << " encumbrance).\n\n";
                } else {
                    std::cout << "I shouldn't drop my currently equipped weapon, lest a foe take me by surprise.\n\n";
                }
            } else if ((object->getName() == "street clothes") && (player->getArmor()->getName() != "street clothes")) {  // don't let player take off clothes while wearing armor
                std::vector<InteractableObject*> inventory = player->getInventory();
                if (std::count(inventory.begin(), inventory.end(), object) > 1) {  // but they have multiple street clothes
                    player->removeFromInventory(object);
                    player->getLocation()->addInteractableObject(object);
                    std::cout << "I open my knapsack and grab the " << stripSpaces(playerInput.substr(5)) << ", dropping it on the ground beside me (-" << object->getEncumbrance() << " encumbrance).\n\n";
                } else {
                    std::cout << "Try as I might, I cannot take off my street clothes while wearing armor over it.\n\n";
                }
            } else if ((object->getName() == "street clothes") && (player->getArmor()->getName() == "street clothes")) {  // don't let player take off clothes
                std::vector<InteractableObject*> inventory = player->getInventory();
                if (std::count(inventory.begin(), inventory.end(), object) > 1) {  // but they have multiple street clothes
                    player->removeFromInventory(object);
                    player->getLocation()->addInteractableObject(object);
                    std::cout << "I open my knapsack and grab the " << stripSpaces(playerInput.substr(5)) << ", dropping it on the ground beside me (-" << object->getEncumbrance() << " encumbrance).\n\n";
                } else {
                    std::cout << "I dare not take off all my clothes in public, lest someone call me a madman and attack me on sight.\n\n";
                }
            } else if (object == player->getArmor()) {
                player->removeFromInventory(object);
                player->getLocation()->addInteractableObject(object);
                std::cout << "I take off the " << player->getArmor()->getName() << ", dropping it on the ground beside me (-" << player->getArmor()->getEncumbrance() << " encumbrance).\n\n";
                player->setArmorToStreetClothes();
            } else if (object == player->getShield()) {
                player->removeFromInventory(object);
                player->setShield(NULL);
                player->getLocation()->addInteractableObject(object);
                std::cout << "I drop the " << stripSpaces(playerInput.substr(5)) << ", leaving my off hand free (-" << object->getEncumbrance() << " encumbrance).\n\n";
            } else {
                player->removeFromInventory(object);
                player->getLocation()->addInteractableObject(object);
                std::cout << "I open my knapsack and grab the " << stripSpaces(playerInput.substr(5)) << ", dropping it on the ground beside me (-" << object->getEncumbrance() << " encumbrance).\n\n";
            }
        } else {
            std::cout << "I cannot find " << addQuotes(stripSpaces(playerInput.substr(4))) << " in my knapsack.\n\n";
        }
    } else {
        std::cout << "\n";
    }
}

void inputLook(Player *player, std::string playerInput) {
    if (playerInput.length() > 5) {
        if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is in current location
            std::cout << player->getLocation()->getObjectFromString(stripSpaces(playerInput.substr(5)))->getDescription() << "\n\n";
        } else if (player->checkStringInInventory(stripSpaces(playerInput.substr(5)))) {  // object is in inventory
            std::cout << player->getObjectFromString(stripSpaces(playerInput.substr(5)))->getDescription() << "\n\n";
        } else {
            std::cout << "I do not see " << addQuotes(stripSpaces(playerInput.substr(5))) << " around me, nor can I find it in my knapsack.\n\n";
        }
    } else {
        printLocationInfo(player);
    }
}

void inputUse(Player *player, std::string playerInput) {
    if (playerInput.length() > 4) {
        if (player->checkStringInInventory(stripSpaces(playerInput.substr(4)))) {  // object is in inventory
            InteractableObject *object = player->getObjectFromString(stripSpaces(playerInput.substr(4)));
            object->use(player);
        } else {
            std::cout << "I cannot find " << addQuotes(stripSpaces(playerInput.substr(4))) << " in my knapsack.\n\n";
        }
    } else {
        std::cout << "\n";
    }
}

void inputAttack(Player *player, std::string playerInput) {
    if (player->isOverEncumbered()) {
        std::cout << "I am carrying too much. I need to drop something before I can prepare myself for combat.\n\n";
    } else if (playerInput.length() > 7) {
        if (player->getLocation()->checkStringInEnemies(stripSpaces(playerInput.substr(7)))) {  // enemy is in current location
            GenericEnemy *enemy = player->getLocation()->getEnemyFromString(stripSpaces(playerInput.substr(7)));
            combat(player, enemy);
        } else {
            std::cout << addQuotes(stripSpaces(playerInput.substr(7))) << " does not look like something I can attack.\n\n";
        }
    } else {
        std::cout << "\n";
    }
}

void printCheckpointCreated() {
    std::cout << "Checkpoint created!\n\n";
}

void printCheckpointLoaded() {
    std::cout << "Last checkpoint loaded!\n\n";
}

void printHelpMessage() {
    /*
    Called if user input is "help" (case insensitive, leading/trailing spaces are ignored). Displays valid user commands.
    */
    std::cout << "\nValid commands: \n";
    std::cout << "    go ___: move in a particular direction\n";
    std::cout << "    take ___: put an object in your location into your inventory\n";
    std::cout << "    drop ___: drop an object from your inventory into your location\n";
    std::cout << "    use ___: use (or attempt to use) an object in your inventory\n";
    std::cout << "    look: inspect your location\n";
    std::cout << "    look ___: inspect a particular object in your location or in your inventory\n";
    std::cout << "    attack ___: attack a potential hostile in your location\n";
    std::cout << "    inventory: view your inventory\n";
    std::cout << "    stats: view your stats\n";
    std::cout << "    journal: view your journal\n\n";
}

void printLocationInfo(Player *player) {
    // general location info
    Location *location = player->getLocation();
    std::cout << "\n-- " << location->getName() << " --\n\n";
    std::cout << location->getDescription() << "\n";
    
    // directions
    std::vector<Direction*> directions = location->getDirections();
    if (directions.size() > 0) {
        std::cout << "\nIt appears I can move in these directions:\n";
        for (size_t i = 0; i < directions.size(); i++) {
            std::cout << "    " << directions.at(i)->getName() << "\n";
        }
    }
    
    // interactable objects
    std::vector<InteractableObject*> objects = location->getObjects();
    if (objects.size() > 0) {
        std::cout << "\nI can spot these items that might be of interest to me:\n";
        std::map<InteractableObject*, int> objectsWithCounts = {};
        for (size_t i = 0; i < objects.size(); i++) {
            InteractableObject *object = objects.at(i);
            objectsWithCounts[object]++;
        }
        for (std::map<InteractableObject*, int>::iterator i = objectsWithCounts.begin(); i != objectsWithCounts.end(); i++) {
            InteractableObject *object = i->first;
            if (i->second > 1)
                std::cout << "    (" << i->second << ") " << object->getName() << "\n";
            else
                std::cout << "    " << object->getName() << "\n";
        }
    }
    
    // enemies
    std::vector<GenericEnemy*> enemies = location->getEnemies();
    if (enemies.size() > 0) {
        std::cout << "\nI should tread carefully; there are potential hostiles here:\n";
        for (size_t i = 0; i < enemies.size(); i++) {
            std::cout << "    " << enemies.at(i)->getName() << "\n";
        }
    }
    
    std::cout << "\n";
}

void printInventory(Player *player) {
    std::cout << "\nI search my knapsack and find the following items (encumbrance in parentheses): \n";
    std::vector<InteractableObject*> inventory = player->getInventory();
    std::map<InteractableObject*, int> inventoryWithCounts = {};
    for (size_t i = 0; i < inventory.size(); i++) {
        InteractableObject *object = inventory.at(i);
        inventoryWithCounts[object]++;
    }
    for (std::map<InteractableObject*, int>::iterator i = inventoryWithCounts.begin(); i != inventoryWithCounts.end(); i++) {
        InteractableObject *object = i->first;
        if (i->second > 1)
            std::cout << "    (" << i->second << ") " << object->getName() << " (" << object->getEncumbrance() << ")\n";
        else
            std::cout << "    " << object->getName() << " (" << object->getEncumbrance() << ")\n";
    }
    std::cout << "\n";
}

void printStats(Player *player) {
    std::cout << "\nHealth: " << player->getCurrentHealth() << "/" << player->getStartingHealth();
    std::cout << "\nEncumbrance: " << player->getTotalEncumbrance() << "/" << MAX_ENCUMBRANCE;
    std::cout << "\nWeapon: " << player->getWeapon()->getName() << "\n" << player->getWeapon()->getStatString();
    std::cout << "\nArmor: " << player->getArmor()->getName() << "\n" << player->getArmor()->getStatString();

    if (!player->getShield())
        std::cout << "\nShield: none";
    else {
        std::cout << "\nShield: " << player->getShield()->getName() << "\n" << player->getShield()->getStatString();
    }

    std::cout << "\n\n";
}

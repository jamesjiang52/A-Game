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

void getUserInput(Player *player) {
    /*
    Gets and performs the player action
    Right now, the program throws and error if the user input is "go", "take", "look", or "use" without a direction/object following the command
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
            printLocationInfo(player);
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
            // TODO: put the object use message in its own use() function
        } else {
            std::cout << "I cannot find " << addQuotes(stripSpaces(playerInput.substr(4))) << " in my knapsack.\n\n";
        }
    } else if (playerInput.substr(0, 6) == "attack") {  // player wants to attack
        // I'll ignore this for now since I haven't thought about how to do combat yet
        ;
    } else if (playerInput == "inventory") {  // inventory
        printInventory(player);
    } else {  // command is not valid
        std::cout << "I do not know that action.\n\n";
    }
}

void printHelpMessage() {
    /*
    Called if user input is "help" (case insensitive, leading/trailing spaces are ignored). Displays valid user commands.
    */
    std::string validCommands[] = {"go ___", "take ___ ", "use ___", "look ___", "attack ___", "inventory"};
    std::cout << "Valid commands: \n";
    
    for (int i = 0; i < 6; i++) {
        std::cout << "    " << validCommands[i] << "\n";
    }
    
    std::cout << "\n";
}

void printLocationInfo(Player *player) {
    Location *location = player->getLocation();
    std::cout << location->getName() << "\n\n";
    std::cout << location->getDescription() << "\n\n";
}

void printInventory(Player *player) {
    std::cout << "I search my knapsack and find the following items: \n";
    std::vector<InteractableObject*> inventory = player->getInventory();
    for (int i = 0; i < inventory.size(); i++) {
        std::cout << "    " << inventory.at(i)->getName() << "\n";
    }
    std::cout << "\n";
}

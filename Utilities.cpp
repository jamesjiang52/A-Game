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
    std::cout << "\n";
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
        if (playerInput.length() > 3) {
            if (player->getLocation()->checkStringInDirections(stripSpaces(playerInput.substr(3)))) {  // direction is valid
                player->setLocation(player->getLocation()->getDirectionFromString(stripSpaces(playerInput.substr(3)))->getDestination());
                printLocationInfo(player);
            } else {
                std::cout << addQuotes(stripSpaces(playerInput.substr(3))) << " does not look like a direction I can travel in.\n\n";
            }
        } else {
            std::cout << "\n";
        }
    } else if (playerInput.substr(0, 4) == "take") {  // player wants to take object
        if (playerInput.length() > 5) {
            if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is valid
                InteractableObject *object = player->getLocation()->getObjectFromString(stripSpaces(playerInput.substr(5)));
                player->addToInventory(object);
                player->getLocation()->removeInteractableObject(object);
                std::cout << "I pick up the " << stripSpaces(playerInput.substr(5)) << " and put it in my knapsack.\n\n";
            } else {
                std::cout << "I do not see " << addQuotes(stripSpaces(playerInput.substr(5))) << ".\n\n";
            }
        } else {
            std::cout << "\n";
        }
    } else if (playerInput.substr(0, 4) == "look") {  // player wants to look at object
        if (playerInput.length() > 5) {
            if (player->getLocation()->checkStringInObjects(stripSpaces(playerInput.substr(5)))) {  // object is in current location
                std::cout << player->getLocation()->getObjectFromString(stripSpaces(playerInput.substr(5)))->getDescription();
            } else if (player->checkStringInInventory(stripSpaces(playerInput.substr(5)))) {  // object is in inventory
                std::cout << player->getObjectFromString(stripSpaces(playerInput.substr(5)))->getDescription();
            } else {
                std::cout << "I do not see " << addQuotes(stripSpaces(playerInput.substr(5))) << " around me, nor can I find it in my knapsack.\n\n";
            }
        } else {
            std::cout << "\n";
        }
    } else if (playerInput.substr(0, 3) == "use") {  // player wants to use object
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
    } else if (playerInput.substr(0, 6) == "attack") {  // player wants to attack
        if (playerInput.length() > 7) {
            if (player->getLocation()->checkStringInEnemies(stripSpaces(playerInput.substr(7)))) {  // enemy is in current location
                GenericEnemy *enemy = player->getLocation()->getEnemyFromString(stripSpaces(playerInput.substr(7)));
                combat(player, enemy);
            } else {
                std::cout << addQuotes(stripSpaces(playerInput.substr(7))) << " does not look like something I can attack.\n\n";
            }
        } else {
            std::cout << "\n";
        }
    } else if (playerInput == "inventory") {  // inventory
        printInventory(player);
    } else if (playerInput == "health") {
        printPlayerHealthInfo(player);
    } else {  // command is not valid
        std::cout << "I do not know that action.\n\n";
    }
}

std::string getEnemyCombatChoice(GenericEnemy *enemy) {
    /*
    This function is the entirety of enemy "AI" in the game
    */
    // just have it random for now
    std::string choices[3] = {"swing", "parry", "feint"};
    std::srand(std::time(nullptr));
    return(choices[std::rand() % 3]);
}

int coinToss() {
    std::srand(std::time(nullptr));
    return(std::rand() % 2);
}

void combat(Player *player, GenericEnemy *enemy) {
    std::string playerInput;
    
    std::cout << "I engage in combat with the " << enemy->getName() << ".\n";
    
    bool playerStaggered = false;  // true for one turn after player swings and enemy parries
    bool playerSurprised = false;  // true for one turn after player parries and enemy feints
    bool enemyStaggered = false;  // true for one turn after enemy swings and player parries
    bool enemySurprised = false;  // true for one turn after enemy parries and player feints
    bool playerFailed = false;
    bool enemyFailed = false;
    
    while (player->getCurrentHealth() > 0 && enemy->getCurrentHealth() > 0) {
        printPlayerHealthInfo(player);
        std::cout << "I can choose to swing, parry, or feint with my " << player->getWeapon()->getName() << ". What do I do?\n";
        std::getline(std::cin, playerInput);
        std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
        playerInput = stripSpaces(playerInput);
        
        std::string enemyChoice = getEnemyCombatChoice(enemy);
        
        if (playerStaggered || playerSurprised) {
            if (coinToss()) {
                playerFailed = true;
            }
            playerStaggered = false;
            playerSurprised = false;
        }
        
        if (enemyStaggered || enemySurprised) {
            if (coinToss()) {
                enemyFailed = true;
            }
            enemyStaggered = false;
            enemySurprised = false;
        }
        
        if (playerInput.substr(0, 2) == "go") {
            std::cout << "I should slay the " << enemy->getName() << " before I proceed.\n\n";
        } else if (playerInput.substr(0, 3) == "take") {
            std::cout << "I should focus on slaying the " << enemy->getName() << " first.\n\n"
        } else if (playerInput.substr(0, 3) == "look") {
            std::cout << "I should focus on slaying the " << enemy->getName() << " first.\n\n";
        } else if (playerInput.substr(0, 3) == "use") {
            std::cout << "I shouldn't try to open my knapsack while I'm in combat.\n\n";
        } else if (playerInput.substr(0, 5) == "attack") {
            std::cout << "I am already in combat with the " << enemy->getName() << ".\n\n";
        } else if (playerInput == "inventory") {
            std::cout << "I shouldn't try to look inside my knapsack while I'm in combat.\n\n";
        } else if (playerInput == "health") {
            printPlayerHealthInfo(player);
        } else if (playerInput == "swing") {
            if (enemyChoice == "swing") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I swing at the " << enemy->getName();
                    player->loseHealth(enemy->getWeapon()->getDamage());
                    enemy->loseHealth(player->getWeapon()->getDamage());
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    enemy->loseHealth(player->getWeapon()->getDamage());
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    player->loseHealth(enemy->getWeapon()->getDamage());
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                    playerStaggered = true;
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    enemy->loseHealth(player->getWeapon()->getDamage());
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    ;
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                    enemy->loseHealth(player->getWeapon()->getDamage());
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    enemy->loseHealth(player->getWeapon()->getDamage());
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    ;
                }
            }
        } else if (playerInput == "parry") {
            if (enemyChoice == "swing") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                    enemyStaggered = true;
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    ;
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    player->loseHealth(enemy->getWeapon()->getDamage());
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    ;
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    ;
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                    playerSurprised = true;
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    ;
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    ;
                }
            }
        } else if (playerInput == "feint") {
            if (enemyChoice == "swing") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                    player->loseHealth(enemy->getWeapon()->getDamage());
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    ;
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    player->loseHealth(enemy->getWeapon()->getDamage());
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                    enemySurprised = true;
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    ;
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    ;
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    // std::cout << ;
                } else if (!playerFailed && enemyFailed) {
                    // std::cout << ;
                    ;
                } else if (playerFailed && !enemyFailed) {
                    // std::cout << ;
                    ;
                }
            }
        } else {  // invalid command
            std::cout << "I do not know that action.\n\n";
        }
        
        playerFailed = false;
        enemyFailed = false;
    }
    
    if (!player->getCurrentHealth()) {  // player dies
        ;
    } else {  // enemy dies
        ;
    }
}

void printHelpMessage() {
    /*
    Called if user input is "help" (case insensitive, leading/trailing spaces are ignored). Displays valid user commands.
    */
    std::string validCommands[] = {"go ___", "take ___ ", "use ___", "look ___", "attack ___", "inventory", "health"};
    std::cout << "\nValid commands: \n";
    
    for (int i = 0; i < 7; i++) {
        std::cout << "    " << validCommands[i] << "\n";
    }
    
    std::cout << "\n";
}

void printLocationInfo(Player *player) {
    Location *location = player->getLocation();
    std::cout << "\n-- " << location->getName() << " --\n\n";
    std::cout << location->getDescription() << "\n";
}

void printInventory(Player *player) {
    std::cout << "\nI search my knapsack and find the following items: \n";
    std::vector<InteractableObject*> inventory = player->getInventory();
    for (int i = 0; i < inventory.size(); i++) {
        std::cout << "    " << inventory.at(i)->getName() << "\n";
    }
    std::cout << "\n";
}

void printPlayerHealthInfo(Player *player) {
    if (player->getCurrentHealth()/(double)player->getStartingHealth() >= 0.9) {
        std::cout << "I am in the best of spirits (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << "health).\n\n";
    } else if (player->getCurrentHealth()/(double)player->getStartingHealth() >= 0.5) {
        std::cout << "I endure the pains and agonies of flesh, yet I stand steadfast (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << "health).\n\n";
    } else if (player->getCurrentHealth()/(double)player->getStartingHealth() >= 0.1) {
        std::cout << "I feel my lifeforce weakened; my next breath is strenuous (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << "health).\n\n";
    } else {
        std::cout << "I desperately cling onto a faint sliver of life, but I feel it fading away (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << "health).\n\n";
    }
}

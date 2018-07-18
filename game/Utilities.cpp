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
    
    if (stripSpaces(playerInput) == "help") {
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
    } else {  // command is not valid
        std::cout << "I do not know that action.\n\n";
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
                std::cout << "I shouldn't drop my currently equipped weapon, lest a foe take me by surprise.\n\n";
            } else if ((object->getName() == "street clothes") && (player->getArmor()->getName() != "street clothes")) {  // don't let player take off clothes while wearing armor
                std::cout << "Try as I might, I cannot take off my street clothes while wearing armor over it.\n\n";
            } else if ((object->getName() == "street clothes") && (player->getArmor()->getName() == "street clothes")) {  // don't let player take off clothes
                std::cout << "I dare not take off all my clothes in public, lest someone call me a madman and attack me on sight.\n\n";
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

std::string getEnemyCombatChoice(GenericEnemy *enemy) {
    /*
    This function is the entirety of enemy "AI" in the game
    */
    // just have it random for now
    std::string choices[3] = {"swing", "parry", "feint"};
    std::srand(std::time(nullptr));
    return(choices[std::rand() % 3]);
}

bool playerCoinToss(Player *player, GenericEnemy *enemy) {
    std::srand(std::time(nullptr));
    int threshold = 100*(MIN_STAGGER_CHANCE + (MAX_STAGGER_CHANCE - MIN_STAGGER_CHANCE)*player->getTotalEncumbrance()/MAX_ENCUMBRANCE);
    
    // stat modifiers
    if (player->getWeapon()) threshold += player->getWeapon()->getPlayerStaggerPercentIncrease();
    if (enemy->getWeapon()) threshold += enemy->getWeapon()->getEnemyStaggerPercentIncrease();
    if (player->getArmor()) threshold += player->getArmor()->getPlayerStaggerPercentIncrease();
    if (enemy->getArmor()) threshold += enemy->getArmor()->getEnemyStaggerPercentIncrease();
    if (player->getShield()) threshold += player->getShield()->getPlayerStaggerPercentIncrease();
    if (enemy->getShield()) threshold += enemy->getShield()->getEnemyStaggerPercentIncrease();
    
    return((std::rand() % 100) < threshold);
}

bool enemyCoinToss(Player *player, GenericEnemy *enemy) {
    std::srand(std::time(nullptr));
    int threshold = 100*(MIN_STAGGER_CHANCE + (MAX_STAGGER_CHANCE - MIN_STAGGER_CHANCE)*enemy->getTotalEncumbrance()/MAX_ENCUMBRANCE);
    
    // stat modifiers
    if (player->getWeapon()) threshold += player->getWeapon()->getEnemyStaggerPercentIncrease();
    if (enemy->getWeapon()) threshold += enemy->getWeapon()->getPlayerStaggerPercentIncrease();
    if (player->getArmor()) threshold += player->getArmor()->getEnemyStaggerPercentIncrease();
    if (enemy->getArmor()) threshold += enemy->getArmor()->getPlayerStaggerPercentIncrease();
    if (player->getShield()) threshold += player->getShield()->getEnemyStaggerPercentIncrease();
    if (enemy->getShield()) threshold += enemy->getShield()->getPlayerStaggerPercentIncrease();
    
    return((std::rand() % 100) < threshold);
}

void combat(Player *player, GenericEnemy *enemy) {
    std::string playerInput;
    
    std::cout << "I engage in combat with the " << enemy->getName() << ".\n";
    
    player->isBleeding = false;
    enemy->isBleeding = false;
    
    bool playerStaggered = false;  // true for one turn after player swings and enemy parries, or player parries and enemy feints
    bool enemyStaggered = false;  // true for one turn after enemy swings and player parries, or enemy parries and player feints
    bool playerFailed = false;
    bool enemyFailed = false;
    
    int combatTurn = 0;
    
    while (player->getCurrentHealth() > 0 && enemy->getCurrentHealth() > 0) {
        printPlayerEmbellishedHealthInfo(player);
        printEnemyEmbellishedHealthInfo(enemy);
        
        player->loseHealth(player->getWeapon()->getPlayerHealthBleed(), 100);
        enemy->loseHealth(enemy->getWeapon()->getPlayerHealthBleed(), 100);
        
        if (player->isBleeding)
            player->loseHealth(enemy->getWeapon()->getEnemyHealthBleed(), 100);
        if (enemy->isBleeding)
            enemy->loseHealth(player->getWeapon()->getEnemyHealthBleed(), 100);
        
        if (!player->getCurrentHealth()) break;
        if (!enemy->getCurrentHealth()) break;
        
        std::cout << "I can choose to swing, parry, or feint with my " << player->getWeapon()->getName() << ". What do I do?\n";
        std::getline(std::cin, playerInput);
        std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
        playerInput = stripSpaces(playerInput);
        
        std::string enemyChoice = getEnemyCombatChoice(enemy);
        
        if (playerStaggered) {
            if (playerCoinToss(player, enemy))
                playerFailed = true;
            playerStaggered = false;
        }
        
        if (enemyStaggered) {
            if (enemyCoinToss(player, enemy))
                enemyFailed = true;
            enemyStaggered = false;
        }
        
        if (playerInput.substr(0, 2) == "go") {
            std::cout << "I should slay the " << enemy->getName() << " before I proceed.\n\n";
        } else if (playerInput.substr(0, 4) == "take") {
            std::cout << "I should focus on slaying the " << enemy->getName() << " first.\n\n";
        } else if (playerInput.substr(0, 4) == "drop") {
            std::cout << "I shouldn't try to open my knapsack while I'm in combat.\n\n";
        } else if (playerInput.substr(0, 4) == "look") {
            std::cout << "I should focus on slaying the " << enemy->getName() << " first.\n\n";
        } else if (playerInput.substr(0, 3) == "use") {
            std::cout << "I shouldn't try to open my knapsack while I'm in combat.\n\n";
        } else if (playerInput.substr(0, 6) == "attack") {
            std::cout << "I am already in combat with the " << enemy->getName() << ".\n\n";
        } else if (playerInput == "inventory") {
            std::cout << "I shouldn't try to look inside my knapsack while I'm in combat.\n\n";
        } else if (playerInput == "stats") {
            printStats(player);
        } else if (playerInput.substr(0, 5)  == "swing") {
            if (enemyChoice == "swing") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I swing at the " << enemy->getName() << ", but I am unable to parry the recoil attack. We exchange blows.\n\n";
                    player->loseHealth(enemy->getWeapon()->getDamage(), enemy->getWeapon()->getEnemyArmorReductionPercent());
                    enemy->loseHealth(player->getWeapon()->getDamage(), player->getWeapon()->getEnemyArmorReductionPercent());
                    player->isBleeding = true;
                    enemy->isBleeding = true;
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I swing at the " << enemy->getName() << ", getting in a powerful strike. It appears a recoil attack was attempted, but thankfully misses.\n\n";
                    enemy->loseHealth(player->getWeapon()->getDamage(), player->getWeapon()->getEnemyArmorReductionPercent());
                    enemy->isBleeding = true;
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "I try to swing at the " << enemy->getName() << ", but unfortunately miss. I suffer a powerful attack in retaliation.\n\n";
                    player->loseHealth(enemy->getWeapon()->getDamage(), enemy->getWeapon()->getEnemyArmorReductionPercent());
                    player->isBleeding = true;
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I swing at the " << enemy->getName() << ", but what would have been a devastating blow is blocked. Slightly staggered, I fear my next action may not be effective.\n\n";
                    playerStaggered = true;
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I swing at the " << enemy->getName() << ". It appears a block was attempted, but I manage to sneak in a hit. Next time, I may not be so lucky.\n\n";
                    enemy->loseHealth(player->getWeapon()->getDamage(), player->getWeapon()->getEnemyArmorReductionPercent());
                    enemy->isBleeding = true;
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "I try to swing at the " << enemy->getName() << ", but unfortunately miss. Luckily, no retaliation was attempted.\n\n";
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I swing at the " << enemy->getName() << ", getting in a powerful strike. I suffer no blow in exchange.\n\n";
                    enemy->loseHealth(player->getWeapon()->getDamage(), player->getWeapon()->getEnemyArmorReductionPercent());
                    enemy->isBleeding = true;
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I swing at the " << enemy->getName() << ", getting in a powerful strike. I suffer no blow in exchange.\n\n";
                    enemy->loseHealth(player->getWeapon()->getDamage(), player->getWeapon()->getEnemyArmorReductionPercent());
                    enemy->isBleeding = true;
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "I try to swing at the " << enemy->getName() << ", but unfortunately miss. Luckily, no retaliation is attempted.\n\n";
                }
            }
        } else if (playerInput.substr(0, 5) == "parry") {
            if (enemyChoice == "swing") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "The " << enemy->getName() << " attacks me, but I parry what would have been a painful blow. I also appear to stagger the " << enemy->getName() << " in the process.\n\n";
                    enemyStaggered = true;
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "The " << enemy->getName() << "tries to attack me, but misses. My preemptive parry was unnecessary, it seems.\n\n"; 
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "The " << enemy->getName() << " attacks me. I try to parry, but I suffer the blow before I can react.\n\n";
                    player->loseHealth(enemy->getWeapon()->getDamage(), enemy->getWeapon()->getEnemyArmorReductionPercent());
                    player->isBleeding = true;
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "Both the " << enemy->getName() << " and I parry preemptively. Caution is the name of the game, it seems.\n\n";
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I preemptively parry an attack from the " << enemy->getName() << ", but it does not come.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "The " << enemy->getName() << " blocks preemptively. Had an attack been attempted instead, I would likely have been too slow to parry.\n\n";
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I parry what I thought was an attack from the " << enemy->getName() << ", but turned out to be a ruse. I stumble in my step, and I fear my next action may not be effective.";
                    playerStaggered = true;
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I preemptively parry an attack from the " << enemy->getName() << ", but it does not come.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "The " << enemy->getName() << " comes at me, and I am too slow to parry if it had been a real attack. Thankfully, it appears to have been a trick, and I quickly regain my step.\n\n";
                }
            }
        } else if (playerInput.substr(0, 5) == "feint") {
            if (enemyChoice == "swing") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I fake a swing at the " << enemy->getName() << ". Unfortunately, the " << enemy->getName() << " is not quite as generous, and I suffer a devastating blow.\n\n";
                    player->loseHealth(enemy->getWeapon()->getDamage(), enemy->getWeapon()->getEnemyArmorReductionPercent());
                    player->isBleeding = true;
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I fake a swing at the " << enemy->getName() << ", who appears to have missed their real attack on me. Next time, I may not be so lucky.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "I try to fake a swing at the " << enemy->getName() << ", but I suffer a blow before I can regain my step.\n\n";
                    player->loseHealth(enemy->getWeapon()->getDamage(), enemy->getWeapon()->getEnemyArmorReductionPercent());
                    player->isBleeding = true;
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I fake a swing at the " << enemy->getName() << ", who falls for my ruse and tries to block. Surprised, the feint also appears to have staggered the " << enemy->getName() << " in the process.\n\n";
                    enemyStaggered = true;
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I fake a swing at the " << enemy->getName() << ". A parry is attempted, but I'm certain that if I had chosen to attack, I would have landed a strike.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "I try to fake a swing at the " << enemy->getName() << ", though admittedly I was not very convincing. Thankfully, no retaliation is attempted.\n\n";
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    std::cout << "I fake a swing at the " << enemy->getName() << ", who fakes their own attack on me as well. The game of trickery is afoot.\n\n";
                } else if (!playerFailed && enemyFailed) {
                    std::cout << "I fake a swing at the " << enemy->getName() << ". It appears that they attempted to fake an attack as well, though it was not terribly impressive.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    std::cout << "I try to fake a swing at the " << enemy->getName() << ", though admittedly I was not very convincing. Thankfully, no retaliation is attempted.\n\n";
                }
            }
        } else {  // invalid command
            std::cout << "I do not know that action.\n\n";
        }
        
        playerFailed = false;
        enemyFailed = false;
        
        combatTurn++;
    }
    
    if (!player->getCurrentHealth()) {  // player dies
        std::cout << "The " << enemy->getName() << ", seeing an opportune moment, lunges at me and steals away the last remnants of my lifeforce with a quick series of swings. I fall to the ground elated, terrified, and broken.\n\n";
        getUserRetryOrQuit();
    } else {  // enemy dies
        std::cout << "I see a perfect opportunity, and I secure another strike on the " << enemy->getName() << ". There is no resistance this time as I watch the blood seep out of the lifeless corpse. While this " << enemy->getName() << " has seen its last breath, I live to fight another day.\n";
        std::cout << "I search the body of the dead " << enemy->getName() << " and find the following items:\n";
        
        for (int i = 0; i < enemy->getInventory().size(); i++) {
            InteractableObject *object = enemy->getInventory().at(i);
            std::cout << "    " << object->getName() << " (" << object->getEncumbrance() << ")\n";
            enemy->removeFromInventory(object);
            player->getLocation()->addInteractableObject(object);
        }
        std::cout << "\n";
        
        delete enemy;
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
    std::string validCommands[] = {"go ___", "take ___ ", "drop ___", "use ___", "look ___", "attack ___", "inventory", "stats"};
    std::cout << "\nValid commands: \n";
    
    for (int i = 0; i < 8; i++) {
        std::cout << "    " << validCommands[i] << "\n";
    }
    std::cout << "\n";
}

void printLocationInfo(Player *player) {
    // general location info
    Location *location = player->getLocation();
    std::cout << "\n-- " << location->getName() << " --\n\n";
    std::cout << location->getDescription();
    
    // directions
    std::vector<Direction*> directions = location->getDirections();
    if (directions.size() > 0) {
        std::cout << "\nIt appears I can move in these directions:" << "\n";
        for (int i = 0; i < directions.size(); i++) {
            std::cout << "    " << directions.at(i)->getName() << "\n";
        }
    }
    
    // interactable objects
    std::vector<InteractableObject*> objects = location->getObjects();
    if (objects.size() > 0) {
        std::cout << "\nI can spot these items that might be of interest to me:" << "\n";
        for (int i = 0; i < objects.size(); i++) {
            std::cout << "    " << objects.at(i)->getName() << "\n";
        }
    }
    
    // enemies
    std::vector<GenericEnemy*> enemies = location->getEnemies();
    if (enemies.size() > 0) {
        std::cout << "\nI should tread carefully; there are potential hostiles here:\n";
        for (int i = 0; i < enemies.size(); i++) {
            std::cout << "    " << enemies.at(i)->getName() << "\n";
        }
    }
    
    std::cout << "\n";
}

void printInventory(Player *player) {
    std::cout << "\nI search my knapsack and find the following items (encumbrance in parentheses): \n";
    std::vector<InteractableObject*> inventory = player->getInventory();
    std::map<InteractableObject*, int> inventoryWithCounts = {};
    for (int i = 0; i < inventory.size(); i++) {
        InteractableObject *object = inventory.at(i);
        inventoryWithCounts[object]++;
    }
    for (std::map<InteractableObject*, int>::iterator i = inventoryWithCounts.begin(); i < inventoryWithCounts.end(); i++) {
        InteractableObject *object = i->second;
        if (i->first > 1)
            std::cout << "    (" << i->first << ") " << object->getName() << " (" << object->getEncumbrance() << ")\n";
        else
            std::cout << object->getName() << " (" << object->getEncumbrance() << ")\n";
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

void printPlayerEmbellishedHealthInfo(Player *player) {
    if (player->getCurrentHealth()/(double)player->getStartingHealth() >= 0.9) {
        std::cout << "I am in the best of spirits, ready to fight";
    } else if (player->getCurrentHealth()/(double)player->getStartingHealth() >= 0.5) {
        std::cout << "I endure the pains and agonies of flesh, yet I stand steadfast";
    } else if (player->getCurrentHealth()/(double)player->getStartingHealth() >= 0.1) {
        std::cout << "I feel my lifeforce weakened; my next breath is strenuous";
    } else {
        std::cout << "I desperately cling onto a faint sliver of life, but I feel it fading away";
    }
    std::cout << " (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << "health).\n";
}

void printEnemyEmbellishedHealthInfo(GenericEnemy *enemy) {
    if (enemy->getCurrentHealth()/(double)enemy->getStartingHealth() >= 0.9) {
        std::cout << "The " << enemy->getName() << " faces me with unbroken courage and vigor";
    } else if (enemy->getCurrentHealth()/(double)enemy->getStartingHealth() >= 0.5) {
        std::cout << "It appears the " << enemy->getName() << " is wavering, though the fire still burns in their eyes";
    } else if (enemy->getCurrentHealth()/(double)enemy->getStartingHealth() >= 0.1) {
        std::cout << "The " << enemy->getName() << " is visibly tormented by the wounds I have inflicted";
    } else {
        std::cout << "It appears the " << enemy->getName() << " is on its last legs";
    }
    std::cout << " (" << enemy->getCurrentHealth() << "/" << enemy->getStartingHealth() << "health).\n\n";
}

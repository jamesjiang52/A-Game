#include "Combat.hpp"

std::string getEnemyCombatChoice(bool weaponDisabled, bool shieldDisabled) {
    /*
    This function is the entirety of enemy "AI" in the game
    */
    // just have it random for now
    if (!weaponDisabled && !shieldDisabled) {
        std::string choices[8] = {"thrust", "slash", "parry", "feint", "dodge", "disengage", "block", "bash"};
        std::srand(std::time(nullptr));
        return(choices[std::rand() % 8]);
    } else if (!weaponDisabled && shieldDisabled) {
        std::string choices[6] = {"thrust", "slash", "parry", "feint", "dodge", "disengage"};
        std::srand(std::time(nullptr));
        return(choices[std::rand() % 6]);
    } else if (weaponDisabled && !shieldDisabled) {
        std::string choices[4] = {"dodge", "disengage", "block", "bash"};
        std::srand(std::time(nullptr));
        return(choices[std::rand() % 4]);
    } else {
        std::string choices[2] = {"dodge", "disengage"};
        std::srand(std::time(nullptr));
        return(choices[std::rand() % 2]);
    }
}

void playerGainHealthFromEffects(Player *player, int combatTurn) {
    int totalModifier = 0;
    
    std::vector<ActiveEffect*> activeEffects = player->getActiveEffects();
    for (int i = 0; i < activeEffects.size(); i++) {
        ActiveEffect *effect = activeEffects.at(i);
        if ((effect->getTargetStat() == "health") && (effect->getCombatTurnsForActivation() == combatTurn)) {
            totalModifier += effect->getAmount();
        }
    }
    player->gainHealth(totalModifier);
    if (!totalModifier)
        return;
    else if (totalModifier > 0)
        std::cout << "Strangely, I feel as if my lifeforce is strengthening. Perhaps it was something I consumed earlier?\n\n";
    else
        std::cout << "Strangely, I feel as if my lifeforce is weakening. Perhaps it was something I consumed earlier?\n\n";
}

int getPlayerStaggerModifierFromEffects(Player *player, int combatTurn) {
    int totalModifier = 0;
    
    std::vector<ActiveEffect*> activeEffects = player->getActiveEffects();
    for (int i = 0; i < activeEffects.size(); i++) {
        ActiveEffect *effect = activeEffects.at(i);
        if ((effect->getTargetStat() == "player stagger") && 
                (effect->getCombatTurnsForActivation() <= combatTurn) && 
                (effect->getCombatTurnsForActivation() + effect->getDuration() >= combatTurn)) {
            totalModifier += effect->getAmount();
        }
    }
    return totalModifier;
}

int getEnemyStaggerModifierFromEffects(Player *player, int combatTurn) {
    int totalModifier = 0;
    
    std::vector<ActiveEffect*> activeEffects = player->getActiveEffects();
    for (int i = 0; i < activeEffects.size(); i++) {
        ActiveEffect *effect = activeEffects.at(i);
        if ((effect->getTargetStat() == "enemy stagger") && 
                (effect->getCombatTurnsForActivation() <= combatTurn) && 
                (effect->getCombatTurnsForActivation() + effect->getDuration() >= combatTurn)) {
            totalModifier += effect->getAmount();
        }
    }
    return totalModifier;
}

void removeInactiveEffects(Player *player, int combatTurn) {
    std::vector<ActiveEffect*> activeEffects = player->getActiveEffects();
    for (int i = 0; i < activeEffects.size(); i++) {
        ActiveEffect *effect = activeEffects.at(i);
        if ((effect->getDuration() >= 0) && (effect->getCombatTurnsForActivation() + effect->getDuration() <= combatTurn)) {
            player->removeFromActiveEffects(effect);
            delete effect;
        }
    }
}

void removeAllEffects(Player *player) {
    /*
    Removes all non-permanent effects. Use at end of combat.
    */
    std::vector<ActiveEffect*> activeEffects = player->getActiveEffects();
    for (int i = 0; i < activeEffects.size(); i++) {
        ActiveEffect *effect = activeEffects.at(i);
        if (effect->getDuration() != -1) {
            player->removeFromActiveEffects(effect);
            delete effect;
        }
    }
}

int getDamageType(int missedModifier, int criticalModifier) {
    /*
    Returns an int indicating the damage type:
        0: missed
        1: critical
        2: normal
    */
    std::srand(std::time(nullptr));
    int roll = std::rand() % 100;
    if (roll < 100*(DEFAULT_MISSED_CHANCE + missedModifier)) 
        return 0;
    else if (roll < 100*(DEFAULT_CRITICAL_CHANCE + DEFAULT_MISSED_CHANCE + missedModifier + criticalModifier))
        return 1;
    return 2;
}

int calculateDamage(int baseDamage, int missedModifier, int criticalModifier, int &damageType) {
    /*
    Determines the amount of damage a player deals, before armor and significant damage calculations.
    */
    int type = getDamageType(missedModifier, criticalModifier);
    
    if (type == 0) {
        damageType = 0;
        return 0;
    } else if (type == 1) {
        damageType = 1;
        return baseDamage*CRITICAL_DAMAGE_MULTIPLIER;
    } else if (type == 2) {
        damageType = 2;
        return baseDamage;
    }
}

bool weaponDisabledRoll(int shieldEncumbrance) {
    std::srand(std::time(nullptr));
    int roll = std::rand() % 100;
    
    int threshold = pow((float)(shieldEncumbrance + BASH_WEAPON_DISABLED_EFFECTIVE_SHIELD_WEIGHT_MODIFIER), BASH_WEAPON_DISABLED_CHANCE_POWER_SCALE)*BASH_WEAPON_DISABLED_CHANCE_MULTIPLIER;
    return(roll < threshold);
}

bool playerStaggerRoll(Player *player, GenericEnemy *enemy, int combatTurn, bool critical, int staggerModifier) {
    /*
    Determines whether player fails their move (from previous turn's stagger)
    */
    std::srand(std::time(nullptr));
    int threshold = 100*(MIN_STAGGER_CHANCE + (MAX_STAGGER_CHANCE - MIN_STAGGER_CHANCE)*player->getTotalEncumbrance()/MAX_ENCUMBRANCE);
    
    // stat modifiers
    if (player->getWeapon()) threshold += player->getWeapon()->getPlayerStaggerPercentIncrease();
    if (enemy->getWeapon()) threshold += enemy->getWeapon()->getEnemyStaggerPercentIncrease();
    if (player->getArmor()) threshold += player->getArmor()->getPlayerStaggerPercentIncrease();
    if (enemy->getArmor()) threshold += enemy->getArmor()->getEnemyStaggerPercentIncrease();
    if (player->getShield()) threshold += player->getShield()->getPlayerStaggerPercentIncrease();
    if (enemy->getShield()) threshold += enemy->getShield()->getEnemyStaggerPercentIncrease();
    
    if (critical)
        return((std::rand() % 100)*SIGNIFICANT_STAGGER_MULTIPLIER + staggerModifier + getPlayerStaggerModifierFromEffects(player, combatTurn) < threshold);
    return((std::rand() % 100) + staggerModifier + getPlayerStaggerModifierFromEffects(player, combatTurn) < threshold);
}

bool enemyStaggerRoll(Player *player, GenericEnemy *enemy, int combatTurn, bool critical, int staggerModifier) {
    /*
    Determines whether enemy fails their move (from previous turn's stagger)
    */
    std::srand(std::time(nullptr));
    int threshold = 100*(MIN_STAGGER_CHANCE + (MAX_STAGGER_CHANCE - MIN_STAGGER_CHANCE)*enemy->getTotalEncumbrance()/MAX_ENCUMBRANCE);
    
    // stat modifiers
    if (player->getWeapon()) threshold += player->getWeapon()->getEnemyStaggerPercentIncrease();
    if (enemy->getWeapon()) threshold += enemy->getWeapon()->getPlayerStaggerPercentIncrease();
    if (player->getArmor()) threshold += player->getArmor()->getEnemyStaggerPercentIncrease();
    if (enemy->getArmor()) threshold += enemy->getArmor()->getPlayerStaggerPercentIncrease();
    if (player->getShield()) threshold += player->getShield()->getEnemyStaggerPercentIncrease();
    if (enemy->getShield()) threshold += enemy->getShield()->getPlayerStaggerPercentIncrease();
    
    if (critical)
        return((std::rand() % 100)*SIGNIFICANT_STAGGER_MULTIPLIER + staggerModifier + getEnemyStaggerModifierFromEffects(player, combatTurn) < threshold);
    return((std::rand() % 100) + staggerModifier + getEnemyStaggerModifierFromEffects(player, combatTurn) < threshold);
}

void resetModifiers(
        int &playerMissedModifier,
        int &playerCriticalModifier,
        int &enemyMissedModifier,
        int &enemyCriticalModifier,
        bool ignorePlayerMissedModifier,
        bool ignorePlayerCriticalModifier,
        bool ignoreEnemyMissedModifier,
        bool ignoreEnemyCriticalModifier,
        bool &playerWeaponDisabled,
        bool &enemyWeaponDisabled,
        bool ignorePlayerWeaponDisabled,
        bool ignoreEnemyWeaponDisabled
    ) {
    if (!ignorePlayerMissedModifier) playerMissedModifier = 0;
    if (!ignorePlayerCriticalModifier) playerCriticalModifier = 0;
    if (!ignoreEnemyMissedModifier) enemyMissedModifier = 0;
    if (!ignoreEnemyCriticalModifier) enemyCriticalModifier = 0;
    if (!ignorePlayerWeaponDisabled) playerWeaponDisabled = false;
    if (!ignoreEnemyWeaponDisabled) enemyWeaponDisabled = false;
}

void combat(Player *player, GenericEnemy *enemy) {
    std::string playerInput;
    
    std::cout << "\nI engage in combat with the " << enemy->getName() << ".\n\n";
    
    bool playerBleeding = false;
    bool enemyBleeding = false;
    
    int playerDamageType = 0;
    int enemyDamageType = 0;
    
    bool playerStaggered = false, playerCriticalStagger = false;
    bool enemyStaggered = false, enemyCriticalStagger = false;
    bool playerFailed = false;
    bool enemyFailed = false;
    
    int combatTurn = 0;
    
    int playerMissedModifier = 0, playerCriticalModifier = 0, playerStaggerModifier = 0;
    int enemyMissedModifier = 0, enemyCriticalModifier = 0, enemyStaggerModifier = 0;
    
    bool playerWeaponDisabled = false, playerShieldDisabled = false;
    bool enemyWeaponDisabled = false, enemyShieldDisabled;
    if (enemy->getShield()) enemyShieldDisabled = false;
    else enemyShieldDisabled = true;
    
    while (player->getCurrentHealth() > 0 && enemy->getCurrentHealth() > 0) {
        printPlayerEmbellishedHealthInfo(player);
        printEnemyEmbellishedHealthInfo(enemy);

        playerGainHealthFromEffects(player, combatTurn);
        player->loseHealth(player->getWeapon()->getPlayerHealthBleed(), 100);
        enemy->loseHealth(enemy->getWeapon()->getPlayerHealthBleed(), 100);

        if (playerBleeding)
            player->loseHealth(enemy->getWeapon()->getEnemyHealthBleed(), 100);
        if (enemyBleeding)
            enemy->loseHealth(player->getWeapon()->getEnemyHealthBleed(), 100);

        if (!player->getCurrentHealth()) break;
        if (!enemy->getCurrentHealth()) break;

        if (!playerWeaponDisabled) {
            std::cout << "I can choose to perform a THRUST, a SLASH, attempt a PARRY, or FEINT a strike with my " << player->getWeapon()->getName();
            std::cout << ". Alternatively, I can perform a DODGE or DISENGAGE the " << enemy->getName();
        } else {
            std::cout << "I can perform a DODGE or DISENGAGE the " << enemy->getName();
        }
        if (player->getShield() && !playerShieldDisabled) {
            std::cout << ", or I can BLOCK or BASH with my " << player->getShield()->getName() << ". What do I do?\n";
        } else {
            std::cout << ". What do I do?\n";
        }
            
        std::getline(std::cin, playerInput);
        std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
        playerInput = stripSpaces(playerInput);
        
        std::string enemyChoice = getEnemyCombatChoice(enemyWeaponDisabled, enemyShieldDisabled);
        
        if (playerStaggered) {
            if (playerStaggerRoll(player, enemy, combatTurn, playerCriticalStagger, playerStaggerModifier))
                playerFailed = true;
            playerStaggered = false;
            playerCriticalStagger = false;
            playerStaggerModifier = 0;
        }
        
        if (enemyStaggered) {
            if (enemyStaggerRoll(player, enemy, combatTurn, enemyCriticalStagger, enemyStaggerModifier))
                enemyFailed = true;
            enemyStaggered = false;
            enemyCriticalStagger = false;
            enemyStaggerModifier = 0;
        }
        
        if (playerInput == "help") {
            printHelpMessage();
        } else if (playerInput.substr(0, 2) == "go") {
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
        } else if (playerInput == "journal") {
            std::cout << "I shouldn't try to open my journal while I'm in combat.\n\n";
        } else if (playerInput == "thrust" && !playerWeaponDisabled) {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        ), 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + PARRY_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    if (enemyDamageType == 0)
                        std::cout << "I lunge underneath the " << enemy->getName() << "'s parry attempt but just barely miss their leg.\n\n";
                    else if (enemyDamageType == 1)
                        std::cout << "I lunge forward, knocking aside the " << enemy->getName() << "'s parry attempt, and sink my " << player->getWeapon()->getName() << " deep into the " << enemy->getName() << "'s thigh.\n\n";
                    else if (enemyDamageType == 2)
                        std::cout << "I lunge forward, knocking aside the " << enemy->getName() << "'s parry attempt, as my " << player->getWeapon()->getName() << " scrapes the " << enemy->getName() << "'s thigh.\n\n";
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    if (enemyDamageType == 0)
                        std::cout << "I lunge underneath the " << enemy->getName() << "'s parry attempt but just barely miss their leg.\n\n";
                    else if (enemyDamageType == 1)
                        std::cout << "I lunge forward, knocking aside the " << enemy->getName() << "'s parry attempt, and sink my " << player->getWeapon()->getName() << " deep into the " << enemy->getName() << "'s thigh.\n\n";
                    else if (enemyDamageType == 2)
                        std::cout << "I lunge forward, and seeing the " << enemy->getName() << " seem to freeze, push my " << player->getWeapon()->getName() << " into their midsection.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "I feel my arm cramping... I must abandon my attack attempt.\n\n";
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "I feel my knees buckling, but luckily the " << enemy->getName() << " seems to have misstepped on a rock and backed off.\n\n";
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyCriticalModifier = THRUST_DODGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, true,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "I lunge forward, but the " << enemy->getName() << " nimbly sidesteps my thrust.\n\n";
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    if (enemyDamageType == 0)
                        std::cout << "I lunge forward and the " << enemy->getName() << " mistakenly steps into the path of my " << player->getWeapon()->getName() << ", but I pull back too soon.\n\n";
                    else if (enemyDamageType == 1)
                        std::cout << "I lunge forward and, seeing the " << enemy->getName() << " stumble, push my " << player->getWeapon()->getName() << " cleanly through their leg.\n\n";
                    else if (enemyDamageType == 2)
                        std::cout << "I lunge forward, and the " << enemy->getName() << " steps right into the path of my " << player->getWeapon()->getName() << ".\n\n";
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "I feel my leg cramping... I must back off.\n\n";
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "A cannon fires in the distance. I jump and shake my head momentarily confused. Luckily, the " << enemy->getName() << " was confused as well.\n\n";
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    if (enemyDamageType == 0)
                        std::cout << "I suddenly notice the " << enemy->getName() << " moving forward and lose concentration, my " << player->getWeapon()->getName() << " going through the air barely centimetres away from their leg.\n\n";
                    else if (enemyDamageType == 1)
                        std::cout << "I lunge forward, knocking aside the " << enemy->getName() << "'s feint, sinking my " << player->getWeapon()->getName() << " into their midsection.\n\n";
                    else if (enemyDamageType == 2)
                        std::cout << "I lunge forward, ignoring the " << enemy->getName() << "'s feint, scoring a gash in their thigh.\n\n";
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    if (enemyDamageType == 0)
                        std::cout << "I suddenly notice the " << enemy->getName() << " moving forward and lose concentration, my " << player->getWeapon()->getName() << " going through the air barely centimetres away from their leg.\n\n";
                    else if (enemyDamageType == 1)
                        std::cout << "I lunge forward, knocking aside the " << enemy->getName() << "'s feint, sinking my " << player->getWeapon()->getName() << " into their midsection.\n\n";
                    else if (enemyDamageType == 2)
                        std::cout << "My quick lunge seems to freeze the " << enemy->getName() << ". I thrust my " << player->getWeapon()->getName() << " deep into their thigh.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "I see the " << enemy->getName() << " moving their blade forward and try to move into a more defensive position, nearly tripping in the process. Luckily, it was only a feint...\n\n";
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "I am startled by a bark of a dog in the distance, but luckily so is the " << enemy->getName() << ".\n\n";
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );

                    std::cout << "I lunge forward, but the " << enemy->getName() << " pulls back out of my reach.\n\n";
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    if (enemyDamageType == 0)
                        std::cout << "I see the " << enemy->getName() << " trip and lunge forward quickly, but am just unable to make contact.\n\n";
                    else if (enemyDamageType == 1)
                        std::cout << "I lunge forward, surprising the " << enemy->getName() << " with my speed, and thrust my sword deep into their thigh.\n\n";
                    else if (enemyDamageType == 2)
                        std::cout << "I notice that the " << enemy->getName() << " is trying to disengage and I quickly strike before they can react.\n\n";
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout < "The " << enemy->getName() << " pulls back, but I overcompensate and barely manage to remain standing.\n\n";
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                    
                    std::cout << "A strange calm falls over us as we stare at each other... contemplating.\n\n";
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyMissedModifier = BLOCK_THRUST_MISSED_MODIFIER;
                    
                    if (weaponDisabledRoll(enemy->getShield()->getEncumbrance()))
                        playerWeaponDisabled = true;
                    else
                        playerWeaponDisabled = false;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        true, false,
                        playerWeaponDisabled, enemyWeaponDisabled, true, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if (playerInput == "slash" && !playerWeaponDisabled) {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER,
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        ), 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyMissedModifier = SLASH_PARRY_MISSED_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        true, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + DODGE_MISSED_MODIFIER + playerMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    enemyCriticalModifier = SLASH_DISENGAGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, true,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyCriticalModifier = BLOCK_SLASH_CRITICAL_MODIFIER;
                    
                    if (weaponDisabledRoll(enemy->getShield()->getEncumbrance()))
                        playerWeaponDisabled = true;
                    else
                        playerWeaponDisabled = false;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, true,
                        playerWeaponDisabled, enemyWeaponDisabled, true, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + playerMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if (playerInput == "parry" && !playerWeaponDisabled) {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            THRUST_MISSED_MODIFIER + enemyMissedModifier,
                            THRUST_CRITICAL_MODIFIER + PARRY_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    playerMissedModifier = SLASH_PARRY_MISSED_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        true, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FEINT_STAGGER_MODIFIER;
                    enemyMissedModifier = PARRY_FEINT_MISSED_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        true, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = true;
                    enemyStaggerModifier = DISENGAGE_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    if (weaponDisabledRoll(enemy->getShield()->getEncumbrance()))
                        playerWeaponDisabled = true;
                    else
                        playerWeaponDisabled = false;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, true, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if (playerInput == "dodge") {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    playerCriticalModifier = THRUST_DODGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, true,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            SLASH_MISSED_MODIFIER + DODGE_MISSED_MODIFIER + enemyMissedModifier,
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER,
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FEINT_STAGGER_MODIFIER;
                    enemyCriticalModifier = FEINT_DODGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, true,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = true;
                    enemyStaggerModifier = SLASH_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = true;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if (playerInput == "feint" && !playerWeaponDisabled) {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            THRUST_MISSED_MODIFIER + enemyMissedModifier,
                            THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            SLASH_MISSED_MODIFIER + enemyMissedModifier,
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FEINT_STAGGER_MODIFIER;
                    playerMissedModifier = PARRY_FEINT_MISSED_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        true, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FEINT_STAGGER_MODIFIER;
                    playerCriticalModifier = FEINT_DODGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, true,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FEINT_STAGGER_MODIFIER;
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FEINT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    enemyMissedModifier = FEINT_DISENGAGE_MISSED_MODIFIER;
                    enemyCriticalModifier = FEINT_DISENGAGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        true, true,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerMissedModifier = BLOCK_FEINT_MISSED_MODIFIER;
                    playerCriticalModifier = BLOCK_FEINT_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        true, true,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if (playerInput == "disengage") {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    playerCriticalModifier = SLASH_DISENGAGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, true,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = true;
                    playerStaggerModifier = DISENGAGE_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = true;
                    playerStaggerModifier = DISENGAGE_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    playerMissedModifier = FEINT_DISENGAGE_MISSED_MODIFIER;
                    playerCriticalModifier = FEINT_DISENGAGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        true, true,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = DISENGAGE_STAGGER_MODIFIER;
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = DISENGAGE_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = DISENGAGE_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if ((playerInput == "block") && player->getShield() && !playerShieldDisabled) {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    playerMissedModifier = BLOCK_THRUST_MISSED_MODIFIER;
                    
                    if (weaponDisabledRoll(player->getShield()->getEncumbrance()))
                        enemyWeaponDisabled = true;
                    else
                        enemyWeaponDisabled = false;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        true, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, true
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    playerCriticalModifier = BLOCK_SLASH_CRITICAL_MODIFIER;
                    
                    if (weaponDisabledRoll(player->getShield()->getEncumbrance()))
                        enemyWeaponDisabled = true;
                    else
                        enemyWeaponDisabled = false;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, true,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, true
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    enemyMissedModifier = BLOCK_FEINT_MISSED_MODIFIER;
                    enemyCriticalModifier = BLOCK_FEINT_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        true, true,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = DISENGAGE_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    playerShieldDisabled = true;
                    enemyShieldDisabled = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if ((playerInput == "bash") && player->getShield() && !playerShieldDisabled) {
            if (enemyChoice == "thrust") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            THRUST_MISSED_MODIFIER + enemyMissedModifier,
                            THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            THRUST_MISSED_MODIFIER + enemyMissedModifier, 
                            THRUST_CRITICAL_MODIFIER + STAGGERED_THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "slash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            THRUST_MISSED_MODIFIER + enemyMissedModifier,
                            THRUST_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            SLASH_MISSED_MODIFIER + STAGGERED_SLASH_MISSED_MODIFIER + enemyMissedModifier, 
                            SLASH_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageType
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    playerBleeding = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    if (weaponDisabledRoll(player->getShield()->getEncumbrance()))
                        enemyWeaponDisabled = true;
                    else
                        enemyWeaponDisabled = false;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, true
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = true;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "disengage") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerShieldDisabled = true;
                    enemyShieldDisabled = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    playerShieldDisabled = true;
                    enemyShieldDisabled = true;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    enemy->loseHealth(
                        pow((float)player->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else if (playerFailed && !enemyFailed) {
                    player->loseHealth(
                        pow((float)enemy->getShield()->getEncumbrance(), BASH_DAMAGE_POWER_SCALE)*BASH_DAMAGE_MULTIPLIER,
                        100
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                } else {
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier,
                        enemyMissedModifier, enemyCriticalModifier,
                        false, false,
                        false, false,
                        playerWeaponDisabled, enemyWeaponDisabled, false, false
                    );
                }
            }
        } else if (playerInput == "thrust" || playerInput == "slash" || playerInput == "parry" || playerInput == "feint" || playerInput == "block" || playerInput == "bash") {
            std::cout << "The state of my equipment doesn't allow me to do that.\n\n";
        } else {  // invalid command
            std::cout << "I do not know that action.\n\n";
        }
        
        playerFailed = false;
        enemyFailed = false;
        
        removeInactiveEffects(player, combatTurn);
        combatTurn++;
    }
    
    if (!player->getCurrentHealth()) {  // player dies
        std::cout << "";  // narrative of enemy killing player
        getUserRetryOrQuit();
    } else {  // enemy dies
        removeAllEffects(player);
    
        std::cout << "";  // narrative of player killing enemy
        
        if (enemy->getInventory().size() > 0) {
            std::cout << "I search the body of the dead " << enemy->getName() << " and find the following items:\n";

            std::vector<InteractableObject*> inventory = enemy->getInventory();
            std::map<InteractableObject*, int> inventoryWithCounts = {};
            for (int i = 0; i < inventory.size(); i++) {
                InteractableObject *object = inventory.at(i);
                inventoryWithCounts[object]++;
                enemy->removeFromInventory(object);
                player->getLocation()->addInteractableObject(object);
            }
            for (std::map<InteractableObject*, int>::iterator i = inventoryWithCounts.begin(); i != inventoryWithCounts.end(); i++) {
                InteractableObject *object = i->first;
                if (i->second > 1)
                    std::cout << "    (" << i->second << ") " << object->getName() << "\n";
                else
                    std::cout << "    " << object->getName() << "\n";
            }  
        }
        std::cout << "\n";
        
        player->getLocation()->removeEnemy(enemy);
        delete enemy;
    }
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
    std::cout << " (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health). ";
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
    std::cout << " (" << enemy->getCurrentHealth() << "/" << enemy->getStartingHealth() << " health).\n\n";
}

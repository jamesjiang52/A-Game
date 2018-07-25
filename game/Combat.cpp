#include "Combat.hpp"

std::string getEnemyCombatChoice(GenericEnemy *enemy) {
    /*
    This function is the entirety of enemy "AI" in the game
    */
    // just have it random for now
    std::string choices[3] = {"swing", "parry", "feint"};
    std::srand(std::time(nullptr));
    return(choices[std::rand() % 3]);
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

int getPlayerStaggerModifierFromEffects(Player *player, GenericEnemy *enemy, int combatTurn) {
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

int getEnemyStaggerModifierFromEffects(Player *player, GenericEnemy *enemy, int combatTurn) {
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

int calculateDamage(int baseDamage, int missedModifier, int criticalModifier, float damageTakenMultiplier) {
    /*
    Determines the amount of damage a player deals, before armor and significant damage calculations.
    */
    int damage = baseDamage;
    
    std::srand(std::time(nullptr));
    int roll = std::rand() % 100;
    if (roll < 100*(DEFAULT_MISSED_CHANCE + missedModifier)) 
        return 0;
    else if (roll < 100*(DEFAULT_CRITICAL_CHANCE + DEFAULT_MISSED_CHANCE + missedModifier + criticalModifier))
        damage *= CRITICAL_DAMAGE_MULTIPLIER;
    return(damage*damageTakenMultiplier);
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
        float &playerDamageTakenMultiplier,
        int &enemyMissedModifier,
        int &enemyCriticalModifier,
        float &enemyDamageTakenMultiplier,
        bool ignorePlayerMissedModifier,
        bool ignorePlayerCriticalModifier,
        bool ignorePlayerDamageTakenMultiplier,
        bool ignoreEnemyMissedModifier,
        bool ignoreEnemyCriticalModifier,
        bool ignoreEnemyDamageTakenMultiplier
    ) {
    if (!ignorePlayerMissedModifier)
        playerMissedModifier = 0;
    if (!ignorePlayerCriticalModifier)
        playerCriticalModifier = 0;
    if (!ignorePlayerDamageTakenMultiplier)
        playerDamageTakenMultiplier = 1;
    if (!ignoreEnemyMissedModifier)
        enemyMissedModifier = 0;
    if (!ignoreEnemyCriticalModifier)
        enemyCriticalModifier = 0;
    if (!ignoreEnemyDamageTakenMultiplier)
        enemyDamageTakenMultiplier = 1;
}

void combat(Player *player, GenericEnemy *enemy) {
    std::string playerInput;
    
    std::cout << "\nI engage in combat with the " << enemy->getName() << ".\n\n";
    
    player->isBleeding = false;
    enemy->isBleeding = false;
    
    bool playerStaggered = false, playerCriticalStagger = false;
    bool enemyStaggered = false, enemyCriticalStagger = false;
    bool playerFailed = false;
    bool enemyFailed = false;
    
    int combatTurn = 0;
    
    int playerMissedModifier = 0, playerCriticalModifier = 0, playerStaggerModifier = 0;
    int enemyMissedModifier = 0, enemyCriticalModifier = 0, enemyStaggerModifier = 0;
    
    float playerDamageTakenMultiplier = 1;
    float enemyDamageTakenMultiplier = 1;
    
    while (player->getCurrentHealth() > 0 && enemy->getCurrentHealth() > 0) {
        printPlayerEmbellishedHealthInfo(player);
        printEnemyEmbellishedHealthInfo(enemy);
        
        playerGainHealthFromEffects(player, combatTurn);

        player->loseHealth(player->getWeapon()->getPlayerHealthBleed(), 100);
        enemy->loseHealth(enemy->getWeapon()->getPlayerHealthBleed(), 100);

        if (player->isBleeding)
            player->loseHealth(enemy->getWeapon()->getEnemyHealthBleed(), 100);
        if (enemy->isBleeding)
            enemy->loseHealth(player->getWeapon()->getEnemyHealthBleed(), 100);

        if (!player->getCurrentHealth()) break;
        if (!enemy->getCurrentHealth()) break;

        std::cout << "I can choose to perform a POWERFUL STRIKE, a LITHE PROBE, attempt a PARRY, or FEINT a strike with my " << player->getWeapon()->getName();
        std::cout << ". Alternatively, I can perform a DODGE or a FALSE RETREAT"
        if (player->getShield()) {
            std::cout << ", or I can BLOCK or BASH with my " << player->getShield()->getName() << ". What do I do?\n";
        } else {
            std::cout << ". What do I do?\n";
        }
            
        std::getline(std::cin, playerInput);
        std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
        playerInput = stripSpaces(playerInput);
        
        std::string enemyChoice = getEnemyCombatChoice(enemy);
        
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
        } else if (playerInput == "powerful strike") {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            POWERFUL_STRIKE_MISSED_MODIFIER + enemyMissedModifier, 
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        ), 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            POWERFUL_STRIKE_MISSED_MODIFIER + playerMissedModifier, 
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            LITHE_PROBE_MISSED_MODIFIER + enemyMissedModifier, 
                            LITHE_PROBE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            POWERFUL_STRIKE_MISSED_MODIFIER + playerMissedModifier, 
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            POWERFUL_STRIKE_MISSED_MODIFIER + playerMissedModifier, 
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + ATTEMPT_PARRY_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = true;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            POWERFUL_STRIKE_MISSED_MODIFIER + playerMissedModifier, 
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    // TODO: chance player has to change weapon
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            POWERFUL_STRIKE_MISSED_MODIFIER + playerMissedModifier, 
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
        } else if (playerInput == "lithe probe") {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            POWERFUL_STRIKE_MISSED_MODIFIER + enemyMissedModifier, 
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            LITHE_PROBE_MISSED_MODIFIER + playerMissedModifier, 
                            LITHE_PROBE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER,
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(), 
                            LITHE_PROBE_MISSED_MODIFIER + enemyMissedModifier, 
                            LITHE_PROBE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        ), 
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    enemy->loseHealth(
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            LITHE_PROBE_MISSED_MODIFIER + playerMissedModifier, 
                            LITHE_PROBE_CRITICAL_MODIFIER + playerCriticalModifierd,
                            enemyDamageTakenMultiplier
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0
                    playerDamageTakenMultiplier = LITHE_PROBE_ATTEMPT_PARRY_ADDITIONAL_DAMAGE_MULTIPLIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, true,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            LITHE_PROBE_MISSED_MODIFIER + DODGE_MISSED_MODIFIER + playerMissedModifier, 
                            LITHE_PROBE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER, 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            LITHE_PROBE_MISSED_MODIFIER + playerMissedModifier, 
                            LITHE_PROBE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    enemyCriticalModifier = LITHE_PROBE_FALSE_RETREAT_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, true, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = true;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    enemy->loseHealth( 
                        calculateDamage(
                            player->getWeapon()->getDamage(), 
                            LITHE_PROBE_MISSED_MODIFIER + playerMissedModifier, 
                            LITHE_PROBE_CRITICAL_MODIFIER + playerCriticalModifier,
                            enemyDamageTakenMultiplier
                        ), 
                        player->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
        } else if (playerInput == "parry") {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            POWERFUL_STRIKE_MISSED_MODIFIER + enemyMissedModifier,
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + ATTEMPT_PARRY_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        ),
                        enemy->getWeapon->getEnemyArmorReductionPercent()
                    );
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    enemyDamageTakenMultiplier = LITHE_PROBE_ATTEMPT_PARRY_ADDITIONAL_DAMAGE_MULTIPLIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, true
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    // nothing
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerStaggerCritical = false;
                    playerStaggerModifier = 0;
                    enemyStaggered = true;
                    enemyStaggerCritical = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FEINT_STAGGER_MODIFIER;
                    enemyMissedModifier = ATTEMPT_PARRY_FEINT_MISSED_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        true, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = true;
                    enemyStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    // TODO: chance player has to change weapon
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
        } else if (playerInput == "dodge") {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = true;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            LITHE_PROBE_MISSED_MODIFIER + DODGE_MISSED_MODIFIER + enemyMissedModifier,
                            LITHE_PROBE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        )*SIGNIFICANT_DAMAGE_MULTIPLIER,
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerStaggerCritical = false;
                    playerStaggerModifier = 0;
                    enemyStaggered = true;
                    enemyStaggerCritical = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    // nothing
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FEINT_STAGGER_MODIFIER;
                    enemyCriticalModifier = FEINT_DODGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, true, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = true;
                    enemyStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
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
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
        } else if (playerInput == "feint") {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            POWERFUL_STRIKE_MISSED_MODIFIER + enemyMissedModifier,
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            LITHE_PROBE_MISSED_MODIFIER + enemyMissedModifier,
                            LITHE_PROBE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FEINT_STAGGER_MODIFIER;
                    playerMissedModifier = ATTEMPT_PARRY_FEINT_MISSED_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        true, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FEINT_STAGGER_MODIFIER;
                    playerCriticalModifier = FEINT_DODGE_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, true, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
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
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FEINT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    playerStaggered = 
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
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
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
        } else if (playerInput == "false retreat") {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    playerCriticalModifier = LITHE_PROBE_FALSE_RETREAT_CRITICAL_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, true, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = true;
                    playerStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = true;
                    playerStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
        } else if ((playerInput == "block") && player->getShield()) {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    // TODO: chance enemy has to change weapon
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = true;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "dodge") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "feint") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = FEINT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = FALSE_RETREAT_STAGGER_MODIFIER;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    // nothing
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    // TODO: both shields are broken and cannot be used for the rest of the fight
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
        } else if ((playerInput == "bash") && player->getShield()) {
            if (enemyChoice == "powerful strike") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            POWERFUL_STRIKE_MISSED_MODIFIER + enemyMissedModifier,
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "lithe probe") {
                if (!playerFailed && !enemyFailed) {
                    player->loseHealth(
                        calculateDamage(
                            enemy->getWeapon()->getDamage(),
                            POWERFUL_STRIKE_MISSED_MODIFIER + enemyMissedModifier,
                            POWERFUL_STRIKE_CRITICAL_MODIFIER + enemyCriticalModifier,
                            playerDamageTakenMultiplier
                        ),
                        enemy->getWeapon()->getEnemyArmorReductionPercent()
                    );
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "parry") {
                if (!playerFailed && !enemyFailed) {
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    // TODO: chance enemy has to change weapon
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
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
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
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
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "false retreat") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "block") {
                if (!playerFailed && !enemyFailed) {
                    // TODO: both shields are broken and cannot be used for the rest of the fight
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            } else if (enemyChoice == "bash") {
                if (!playerFailed && !enemyFailed) {
                    playerStaggered = true;
                    playerCriticalStagger = false;
                    playerStaggerModifier = 0;
                    enemyStaggered = true;
                    enemyCriticalStagger = false;
                    enemyStaggerModifier = 0;
                    
                    resetModifiers(
                        playerMissedModifier, playerCriticalModifier, playerDamageTakenMultiplier,
                        enemyMissedModifier, enemyCriticalModifier, enemyDamageTakenMultiplier,
                        false, false, false,
                        false, false, false
                    );
                } else if (!playerFailed && enemyFailed) {
                    
                } else if (playerFailed && !enemyFailed) {
                    
                } else {
                    
                }
            }
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
    std::cout << " (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).";
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

#include "WeaponClass.hpp"

Weapon::Weapon(
        std::string name, 
        std::string description,
        int encumbrance,
        int damage,
        int playerStaggerPercentIncrease,
        int enemyStaggerPercentIncrease,
        int enemyArmorReductionPercent,
        int playerHealthBleed,
        int enemyHealthBleed,
        bool twoHanded,
        ActiveEffect *activeEffect
    ): InteractableObject(name, description, encumbrance) {
    this->damage = damage;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
    this->enemyArmorReductionPercent = enemyArmorReductionPercent;
    this->playerHealthBleed = playerHealthBleed;
    this->enemyHealthBleed = enemyHealthBleed;
    this->twoHanded = twoHanded;
    this->activeEffect = activeEffect;
}

Weapon::~Weapon() {
    delete activeEffect;
}

int Weapon::getDamage() const {
    return damage;
}

int Weapon::getPlayerStaggerPercentIncrease() const {
    return playerStaggerPercentIncrease;
}

int Weapon::getEnemyStaggerPercentIncrease() const {
    return enemyStaggerPercentIncrease;
}

int Weapon::getEnemyArmorReductionPercent() const {
    return enemyArmorReductionPercent;
}

int Weapon::getPlayerHealthBleed() const {
    return playerHealthBleed;
}

int Weapon::getEnemyHealthBleed() const {
    return enemyHealthBleed;
}

std::string Weapon::getStatString() const {
    /*
    Returns a string with weapon stats. Each stat, beginning with damage, is separated by a newline and begins
    with four spaces. Leading and trailing newlines to the entire statstring are absent.
    */
    std::string stats = "    ";
    
    stats += std::to_string(damage) + " damage";
    if (playerStaggerPercentIncrease > 0)
        stats += "\n    +" + std::to_string(playerStaggerPercentIncrease) + "% player stagger";
    else if (playerStaggerPercentIncrease < 0)
        stats += "\n    -" + std::to_string(playerStaggerPercentIncrease) + "% player stagger";
    if (enemyStaggerPercentIncrease > 0)
        stats += "\n    +" + std::to_string(enemyStaggerPercentIncrease) + "% enemy stagger";
    else if (enemyStaggerPercentIncrease < 0)
        stats += "\n    -" + std::to_string(enemyStaggerPercentIncrease) + "% enemy stagger";
    if (enemyArmorReductionPercent > 0)
        stats += "\n    -" + std::to_string(enemyArmorReductionPercent) + "% enemy armor";
    else if (enemyArmorReductionPercent < 0)
        stats += "\n    +" + std::to_string(enemyArmorReductionPercent) + "% enemy armor";
    if (playerHealthBleed)
        stats += "\n    " + std::to_string(playerHealthBleed) + " persistent damage to player";
    if (enemyHealthBleed)
        stats += "\n    " + std::to_string(enemyHealthBleed) + " persistent damage to enemy";
    
    return stats;
}

bool Weapon::isTwoHanded() const {
    return twoHanded;
}

ActiveEffect *Weapon::getActiveEffect() const {
    return activeEffect;
}

void Weapon::use(Player *player) {
    if (player->getWeapon() != this) {
        player->removeFromActiveEffects(player->getWeapon()->getActiveEffect());
        player->addToActiveEffects(activeEffect);
        
        if (player->getWeapon()->isTwoHanded() && twoHanded) {  // both previous weapon and current weapon are two-handed
            std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << ".\n\n";
            player->setShield(NULL);
        } else if (!player->getWeapon()->isTwoHanded() && twoHanded) {  // previous weapon is one-handed but current weapon is two-handed
            if (player->getShield()) {  // player has shield equipped
                std::cout << "I sheathe the " << player->getWeapon()->getName() << ", put the " << player->getShield()->getName() << " back into my knapsack, and equip the " << name << ".\n\n";
                player->setShield(NULL);
            } else {
                std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << ".\n\n";
                player->setShield(NULL);
            }
        } else if (player->getWeapon()->isTwoHanded() && !twoHanded) {  // previous weapon is two-handed but current weapon is one-handed
            std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << ", freeing up my off hand.\n\n";
            player->setShield(NULL);
        } else {  // both previous weapon and current weapon are one-handed
            std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << ".\n\n";
        }
        player->setWeapon(this);
    } else {  // weapon already equipped
        std::cout << "I already have the " << name << " equipped.\n\n";
    }
}

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
        bool twoHanded
    ): InteractableObject(name, description, encumbrance) {
    this->damage = damage;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
    this->enemyArmorReductionPercent = enemyArmorReductionPercent;
    this->playerHealthBleed = playerHealthBleed;
    this->enemyHealthBleed = enemyHealthBleed;
    this->twoHanded = twoHanded;
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

bool Weapon::isTwoHanded() const {
    return twoHanded;
}

void Weapon::use(Player *player) {
    if (player->getWeapon() != this) {
        if (player->getWeapon()->isTwoHanded() && twoHanded) {  // both previous weapon and current weapon are two-handed
            std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << " (" << damage << " damage).\n\n";
        } else if (!player->getWeapon()->isTwoHanded() && twoHanded) {  // previous weapon is one-handed but current weapon is two-handed
            if (player->getShield()) {  // player has shield equipped
                std::cout << "I sheathe the " << player->getWeapon()->getName() << ", put the " << player->getShield()->getName() << " back into my knapsack, and equip the " << name << " (" << damage << " damage).\n\n";
            } else {
                std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << " (" << damage << " damage).\n\n";
            }
        } else if (player->getWeapon()->isTwoHanded() && !twoHanded) {  // previous weapon is two-handed but current weapon is one-handed
            std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << ", freeing up my off hand (" << damage << " damage).\n\n";
        } else {  // both previous weapon and current weapon are one-handed
            std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << " (" << damage << " damage).\n\n";
        }
        player->setWeapon(this);
    } else {  // weapon already equipped
        std::cout << "I already have the " << name << " equipped (" << damage << " damage).\n\n";
    }
}

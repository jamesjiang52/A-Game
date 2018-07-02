#include "ShieldClass.hpp"

Shield::Shield(std::string name, std::string description, int encumbrance, int armor, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease): InteractableObject(name, description, encumbrance) {
    this->armor = armor;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
}

int Shield::getArmor() const {
    return armor;
}

int Shield::getPlayerStaggerPercentIncrease() const {
    return playerStaggerPercentIncrease;
}

int Shield::getEnemyStaggerPercentIncrease() const {
    return enemyStaggerPercentIncrease;
}

void Shield::use(Player *player) {
    if (!player->canUseShield()) {
        std::cout << "The " << player->getWeapon()->getName() << " requires both of my hands to wield. I must switch to something different if I want to use a shield.\n\n";
    } else if (player->getShield()) {  // no shield currently equipped
        std::cout << "I equip the " << name << " in my off hand (" << armor <<  " armor).\n\n";
        player->setShield(this);
    } else if (player->getShield() != this) {
        std::cout << "I put the " << player->getShield()->getName << " back into my knapsack and equip the " << name << " in my off hand (" << armor << " armor).\n\n";
    } else {  // shield is already equipped
        std::cout << "I already have the " << name << " equipped (" << armor << " armor).\n\n";
    }
}

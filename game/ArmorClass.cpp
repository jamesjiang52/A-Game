#include "ArmorClass.hpp"

Armor::Armor(std::string name, std::string description, int encumbrance, int armor, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease): InteractableObject(name, description, encumbrance) {
    this->armor = armor;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
}

int Armor::getArmor() const {
    return armor;
}

int Armor::getPlayerStaggerPercentIncrease() const {
    return playerStaggerPercentIncrease;
}

int Armor::getEnemyStaggerPercentIncrease() const {
    return enemyStaggerPercentIncrease;
}

void Armor::use(Player *player) {
    if (name == "street clothes") {
        if (player->getArmor() == this) {
            std::cout << "I'm already wearing the street clothes.\n\n";
        } else {
            std::cout << "I'm already wearing the street clothes inside my " << player->getArmor()->getName() << ".\n\n";
        }
    } else if (player->getArmor() != this) {
        std::cout << "I take off the " << player->getArmor()->getName() << " and put on the " << name << " (" << armor << " armor).\n\n";
        player->setArmor(this);
    } else {
        std::cout << "I'm already wearing the " << name << " (" << armor << " armor).\n\n";
    }
}

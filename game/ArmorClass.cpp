#include "ArmorClass.hpp"

Armor::Armor(std::string name, std::string description, int encumbrance, int armor, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease): InteractableObject(name, description, encumbrance) {
    this->armor = armor;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
}

int Armor::getArmor() {
    return armor;
}

int Armor::getPlayerStaggerPercentIncrease() {
    return playerStaggerPercentIncrease;
}

int Armor::getEnemyStaggerPercentIncrease() {
    return enemyStaggerPercentIncrease;
}

void Armor::use(Player *player) {
    if (player->getArmor() != this) {
        if (player->getArmor()->getName() == "street clothes")
            std::cout << "I slip on the " << name << " over my clothes (" << armor << " armor).\n\n";
        else
            std::cout << "I take off the " << player->getArmor()->getName() << " and put on the " << name << " (" << armor << " armor).\n\n";
        player->setArmor(this);
    } else {
        std::cout << "I'm already wearing the " << name << " (" << armor << " armor).\n\n";
    }
}

#include "WeaponClass.hpp"

Weapon::Weapon(std::string name, std::string description, int encumbrance, int damage, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease): InteractableObject(name, description, encumbrance) {
    this->damage = damage;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
}

int Weapon::getDamage() {
    return damage;
}

int Weapon::getPlayerStaggerPercentIncrease() {
    return playerStaggerPercentIncrease;
}

int Weapon::getEnemyStaggerPercentIncrease() {
    return enemyStaggerPercentIncrease;
}

void Weapon::use(Player *player) {
    if (player->getWeapon() != this) {
        std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << " (" << damage << " damage).\n\n";
        player->setWeapon(this);
    } else {
        std::cout << "I already have the " << name << " equipped (" << damage << " damage).\n\n";
    }
}

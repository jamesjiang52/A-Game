#include "WeaponClass.hpp"

Weapon::Weapon(std::string name, std::string description, int encumbrance, int damage): InteractableObject(name, description, encumbrance) {
    this->damage = damage;
}

int Weapon::getDamage() {
    return damage;
}

void Weapon::use(Player *player) {
    if (player->getWeapon() != this) {
        std::cout << "I sheathe the " << player->getWeapon()->getName() << " and equip the " << name << " (" << damage << " damage).\n\n";
        player->setWeapon(this);
    } else {
        std::cout << "I already have the " << name << " equipped (" << damage << " damage).\n\n";
    }
}

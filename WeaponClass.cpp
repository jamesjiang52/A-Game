#include "WeaponClass.hpp"

Weapon::Weapon(std::string name, std::string description, int damage): InteractableObject(name, description) {
    this->damage = damage;
}

int Weapon::getDamage() {
    return damage;
}

void Weapon::use(Player *player) {
    if (player->getWeapon != this) {
        player->setWeapon(this);
        std::cout << "I equip the " << name << " (" << damage << " damage).\n\n";
    } else {
        std::cout << "I already have the " << name << " equipped (" << damage << " damage).\n\n";
    }
}

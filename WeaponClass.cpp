#include "WeaponClass.hpp"

Weapon::Weapon(std::string name, std::string description, int damage): InteractableObject(name, description) {
    this->damage = damage;
}

int Weapon::getDamage() {
    return damage;
}

void Weapon::use(Player *player) {
    player->setWeapon(this);
}

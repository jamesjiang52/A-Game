#include "WeaponClass.hpp"

Weapon::Weapon(std::string name, std::string useMessage, int damage): InteractableObject(name, useMessage) {
    this->damage = damage;
}

int Weapon::getDamage() {
    return damage;
}

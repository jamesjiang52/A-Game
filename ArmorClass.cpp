#include "ArmorClass.hpp"

Armor::Armor(std::string name, std::string useMessage, int armor): InteractableObject(name, useMessage) {
    this->armor = armor;
}

int Armor::getArmor() {
    return armor;
}

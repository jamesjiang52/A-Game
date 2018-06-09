#include "ArmorClass.hpp"

Armor::Armor(std::string name, std::string description, int armor): InteractableObject(name, description) {
    this->armor = armor;
}

int Armor::getArmor() {
    return armor;
}

void Armor::use(Player *player) {
    player->setArmor(this);
    std::cout << "I put on the " << name << " (" << armor << " armor).\n\n";
}

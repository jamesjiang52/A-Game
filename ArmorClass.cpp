#include "ArmorClass.hpp"

Armor::Armor(std::string name, std::string description, int encumbrance, int armor): InteractableObject(name, description, encumbrance) {
    this->armor = armor;
}

int Armor::getArmor() {
    return armor;
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

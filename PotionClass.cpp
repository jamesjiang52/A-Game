#include "PotionClass.hpp"

Potion::Potion(std::string name, std::string description, int healAmount): InteractableObject(name, description) {
    this->healAmount = healAmount;
}

int Potion::getHealAmount() {
    return healAmount;
}

void Potion::use(Player *player) {
    player->loseHealth(-healAmount);
}

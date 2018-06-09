#include "PotionClass.hpp"

Potion::Potion(std::string name, std::string description, int healAmount): InteractableObject(name, description) {
    this->healAmount = healAmount;
}

int Potion::getHealAmount() {
    return healAmount;
}

void Potion::use(Player *player) {
    player->gainHealth(healAmount);
    std::cout << "I feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
    
    // destroy potion
    player->removeFromInventory(this);
    delete this;
}

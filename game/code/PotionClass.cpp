#include "PotionClass.hpp"

Potion::Potion(std::string name, std::string description, int encumbrance, std::function<void(Player *player)> useFunction): InteractableObject(name, description, encumbrance), useFunction(useFunction) {}

void Potion::use(Player *player) {
    useFunction(player);

    // destroy potion
    player->removeFromInventory(this);
    delete this;
}

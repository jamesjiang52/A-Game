#include "PotionClass.hpp"

Potion::Potion(std::string name, std::string description, int encumbrance, const function<void()>& useFunction): InteractableObject(name, description, encumbrance), useFunction(useFunction) {}

void Potion::use(Player *player) {
    useFunction(player);

    // destroy potion
    player->removeFromInventory(this);
    delete this;
}

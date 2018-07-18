#include "MiscObjectClass.hpp"

MiscObject::MiscObject(std::string name, std::string description, int encumbrance, std::function<void(Player *player)> useFunction): InteractableObject(name, description, encumbrance), useFunction(useFunction) {}

void MiscObject::use(Player *player) {
    useFunction(player);
}

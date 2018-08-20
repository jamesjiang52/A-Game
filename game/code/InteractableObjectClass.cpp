#include "InteractableObjectClass.hpp"

InteractableObject::InteractableObject(std::string name, std::string description, int encumbrance) {
    this->name = name;
    this->description = description;
    this->encumbrance = encumbrance;
}

bool InteractableObject::operator==(const InteractableObject &object) {
    return(name == object.getName());
}

std::string InteractableObject::getName() const {
    return name;
}

std::string InteractableObject::getDescription() const {
    return description;
}

void InteractableObject::appendToDescription(std::string string) {
    description += string;
}

int InteractableObject::getEncumbrance() const {
    return encumbrance;
}

void InteractableObject::setUseMessage(std::string useMessage) {  // only use this function for instances of this base class
    this->useMessage = useMessage;
}

void InteractableObject::use(Player *player) {
    std::cout << useMessage << "\n\n";
}

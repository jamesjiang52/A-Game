#include "InteractableObjectClass.hpp"

InteractableObject::InteractableObject(std::string name, std::string description) {
    this->name = name;
    this->description = description;
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

void InteractableObject::setUseMessage(std::string useMessage) {  // only use this function for instances of this base class
    this->useMessage = useMessage;
}

void InteractableObject::use(Player *player) {
    std::cout << useMessage << "\n\n";
}

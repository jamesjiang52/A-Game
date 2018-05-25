#include "InteractableObjectClass.hpp"

InteractableObject::InteractableObject(std::string name, std::string useMessage) {
    this->name = name;
    this->useMessage = useMessage;
}

bool InteractableObject::operator==(const InteractableObject &object) {
    return(name == object.getName());
}

std::string InteractableObject::getName() const {
    return name;
}

std::string InteractableObject::getUseMessage() const {
    return useMessage;
}

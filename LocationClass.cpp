#include "LocationClass.hpp"

Location::Location(std::string name, std::string description) {
    this->name = name;
    this->description = description;
}

std::string Location::getName() const {
    return name;
}

std::string Location::getDescription() const {
    return description;
}

std::vector<Direction> Location::getDirections() const {
    return allowedDirections;
}

std::vector<InteractableObject*> Locations::getObjects() const {
    return interactableObjects;
}

std::vector<Direction>::iterator Location::directionPosition(Direction direction) {
    /* 
    Returns a vector iterator, whose value is in the interval [0, size(allowedDirections)) if direction
    is in allowedDirections, and size(allowedDirections) if direction is NOT in allowedDirections
    */
    return std::find(allowedDirections.begin(), allowedDirections.end(), direction);
}

void Location::addDirection(Direction direction) {
    /*
    Adds new direction to allowedDirections if it is not already in it
    */
    if (directionPosition(direction) == allowedDirections.end())
        allowedDirections.push_back(direction);
}

void Location::removeDirection(Direction direction) {
    /*
    Removes direction from allowedDirections if it is in it
    Otherwise, does nothing
    */
    std::vector<Direction>::iterator position = directionPosition(direction);
    if (position != allowedDirections.end())
        allowedDirections.erase(position);
}

std::vector<InteractableObject*>::iterator Location::objectPosition(InteractableObject object) {
    /* 
    Returns a vector iterator, whose value is in the interval [0, size(interactableObjects)) if object
    is in interactableObjects, and size(interactableObjects) if object is NOT in interactableObjects
    */
    return std::find(interactableObjects.begin(), interactableObjects.end(), &object);
)

void Location::addInteractableObject(InteractableObject object) {
    /*
    Adds new object to interactableObjects if it is not already in it
    */
    if (objectPosition(object) == interactableObjects.end())
        interactableObjects.push_back(&object);
}

void Location::removeInteractableObject(InteractableObject object) {
    /*
    Removes object from interactableObjects if it is in it
    Otherwise, does nothing
    */
    std::vector<InteractableObject>::iterator position = objectPosition(object);
    if (position != interactableObjects.end())
        interactableObjects.erase(position);
}
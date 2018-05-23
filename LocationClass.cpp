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

std::vector<Direction>::iterator Location::position(Direction direction) {
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
    if (position(direction) == allowedDirections.end())
        allowedDirections.push_back(direction);
}

void Location::removeDirection(Direction direction) {
    /*
    Removes direction from allowedDirections if it is in it
    Otherwise, does nothing
    */
    if (position(direction) != allowedDirections.end())
        allowedDirections.erase(position(direction));
}

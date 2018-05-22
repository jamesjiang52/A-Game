#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>
#include <algorithm>
#include "DirectionClass.hpp"

class Location {
    private:
        std::string name;
        std::string description;
        std::vector<Direction> allowedDirections;
        
    public:
        std::string getName();
        std::string getDescription();
        std::vector<Direction>::iterator position(Direction direction);
        void addDirection(Direction direction);
        void removeDirection(Direction direction);
}

Location::Location(std::string name, std::string description) {
    this->name = name;
    this->description = description;
}

std::string Location::getName() {
    return name;
}

std::string Location::getDescription() {
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
    if (position(direction) != allowedDirections.end())
        allowedDirections.erase(position(direction));
}

#endif

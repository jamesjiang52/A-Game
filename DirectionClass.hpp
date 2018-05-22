#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>
#include "LocationClass.hpp"

class Direction {
    private:
        std::string name;
        Location destination;
        
    public:
        std::string getName();
        Location getDestination();
}

Direction::Direction(std::string name, Location destination) {
    this->name = name;
    this->destination = destination;
}

std::string Direction::getName() {
    return name;
}

Location Direction::getDestination() {
    return destination;
}

#endif

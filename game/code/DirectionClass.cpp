#include "DirectionClass.hpp"

Direction::Direction(std::string name, Location *destination) {
    this->name = name;
    this->destination = destination;
}

bool Direction::operator==(const Direction &direction) {
    return(name == direction.getName());
}

std::string Direction::getName() const {
    return name;
}

Location *Direction::getDestination() const {
    return destination;
}

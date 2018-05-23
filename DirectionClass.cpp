#include "DirectionClass.hpp"

Direction::Direction(std::string name, Location *destination) {
    this->name = name;
    this->destination = destination;
}

std::string Direction::getName() const {
    return name;
}

Location *Direction::getDestination() const {
    return destination;
}

bool Direction::operator==(const Direction &direction) {
    bool namesEqual = (name == direction.getName());
    bool destinationsEqual = (destination == direction.getDestination());
    return(namesEqual && destinationsEqual);
}

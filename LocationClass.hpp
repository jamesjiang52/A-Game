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
        Location(std::string name, std::string description);
        std::string getName() const;
        std::string getDescription() const;
        std::vector<Direction>::iterator position(Direction direction);
        void addDirection(Direction direction);
        void removeDirection(Direction direction);
};

#endif

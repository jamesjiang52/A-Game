#ifndef DIRECTION_H
#define DIRECTION_H

#include <string>

class Location;

class Direction {
    private:
        std::string name;
        Location *destination;
        
    public:
        Direction(std::string name, Location *destination);
        bool operator==(const Direction &direction);
        std::string getName() const;
        Location *getDestination() const;
};

#endif

#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>
#include <algorithm>
#include "DirectionClass.hpp"
#include "InteractableObjectClass.hpp"

class Location {
    private:
        std::string name;
        std::string description;
        std::vector<Direction*> allowedDirections;
        std::vector<InteractableObject*> interactableObjects;
        
    public:
        Location(std::string name, std::string description);
        std::string getName() const;
        std::string getDescription() const;
        std::vector<Direction*> getDirections() const;
        std::vector<InteractableObject*> getObjects() const;
        std::vector<Direction*>::iterator directionPosition(Direction *direction);
        bool checkStringInDirections(std::string directionName);
        bool checkStringInObjects(std::string objectName);
        void addDirection(Direction *direction);
        void removeDirection(Direction *direction);
        std::vector<InteractableObject*>::iterator objectPosition(InteractableObject *object);
        void addInteractableObject(InteractableObject *object);
        void removeInteractableObject(InteractableObject *object);
};

#endif

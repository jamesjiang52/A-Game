#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>
#include <algorithm>
#include "DirectionClass.hpp"
#include "InteractableObjectClass.hpp"
#include "GenericEnemyClass.hpp"

class GenericEnemy;

class Location {
    private:
        std::string name;
        std::string description;
        std::vector<Direction*> allowedDirections;
        std::vector<InteractableObject*> interactableObjects;
        std::vector<GenericEnemy*> enemies;
        std::vector<Direction*>::iterator directionPosition(Direction *direction);
        std::vector<InteractableObject*>::iterator objectPosition(InteractableObject *object);
        std::vector<GenericEnemy*>::iterator enemyPosition(GenericEnemy *enemy);
        
    public:
        Location(std::string name, std::string description);
        ~Location();
        bool operator==(const Location &location);
        std::string getName() const;
        std::string getDescription() const;
        std::vector<Direction*> getDirections() const;
        std::vector<InteractableObject*> getObjects() const;
        std::vector<GenericEnemy*> getEnemies() const;
        void addDirection(Direction *direction);
        void removeDirection(Direction *direction);
        bool checkStringInDirections(std::string directionName);
        Direction *getDirectionFromString(std::string directionName);
        void addInteractableObject(InteractableObject *object);
        void removeInteractableObject(InteractableObject *object);
        bool checkStringInObjects(std::string objectName);
        InteractableObject *getObjectFromString(std::string objectName);
        void addEnemy(GenericEnemy *enemy);
        void removeEnemy(GenericEnemy *enemy);
        bool checkStringInEnemies(std::string enemyName);
        GenericEnemy *getEnemyFromString(std::string enemyName);
};

#endif

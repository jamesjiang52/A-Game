#include "LocationClass.hpp"

Location::Location(std::string name, std::string description) {
    this->name = name;
    this->description = description;
}

bool Location::operator==(const Location &location) {
    return(name == location.getName());
}

std::string Location::getName() const {
    return name;
}

std::string Location::getDescription() const {
    return description;
}

std::vector<Direction*> Location::getDirections() const {
    return allowedDirections;
}

std::vector<InteractableObject*> Location::getObjects() const {
    return interactableObjects;
}

std::vector<GenericEnemy*> Location::getEnemies() const {
    return enemies;
}

std::vector<Direction*>::iterator Location::directionPosition(Direction *direction) {
    /* 
    Returns a vector iterator, with value corresponding to the index of the allowedDirections vector
    if direction is in the vector. Otherwise, returns the size of allowedDirections.
    */
    return std::find(allowedDirections.begin(), allowedDirections.end(), direction);
}

void Location::addDirection(Direction *direction) {
    /*
    Adds new direction to allowedDirections if it is not already in it
    */
    if (directionPosition(direction) == allowedDirections.end())
        allowedDirections.push_back(direction);
}

void Location::removeDirection(Direction *direction) {
    /*
    Removes direction from allowedDirections if it is in it
    Otherwise, does nothing
    */
    std::vector<Direction*>::iterator position = directionPosition(direction);
    if (position != allowedDirections.end())
        allowedDirections.erase(position);
}

bool Location::checkStringInDirections(std::string directionName) {
    /*
    Checks if the string corresponds to a location name in allowedDirections
    */
    for (int i = 0; i < allowedDirections.size(); i++) {
        if (directionName == allowedDirections.at(i)->getName())
            return true;
    }
    return false;
}

Direction *Location::getDirectionFromString(std::string directionName) {
    /*
    Returns a reference to the direction given by directionName
    */
    for (int i = 0; i < allowedDirections.size(); i++) {
        if (directionName == allowedDirections.at(i)->getName())
            return allowedDirections.at(i);
    }
}

std::vector<InteractableObject*>::iterator Location::objectPosition(InteractableObject *object) {
    /* 
    Returns a vector iterator, with value corresponding to the index of the interactableObjects vector
    if object is in the vector. Otherwise, returns the size of interactableObjects.
    */
    return std::find(interactableObjects.begin(), interactableObjects.end(), object);
}

void Location::addInteractableObject(InteractableObject *object) {
    /*
    Adds new object to interactableObjects if it is not already in it
    */
    if (objectPosition(object) == interactableObjects.end())
        interactableObjects.push_back(object);
}

void Location::removeInteractableObject(InteractableObject *object) {
    /*
    Removes object from interactableObjects if it is in it
    Otherwise, does nothing
    */
    std::vector<InteractableObject*>::iterator position = objectPosition(object);
    if (position != interactableObjects.end())
        interactableObjects.erase(position);
}

bool Location::checkStringInObjects(std::string objectName) {
    /*
    Checks if the string corresponds to an object name in interactableObjects
    */
    for (int i = 0; i < interactableObjects.size(); i++) {
        if (objectName == interactableObjects.at(i)->getName())
            return true;
    }
    return false;
}

InteractableObject *Location::getObjectFromString(std::string objectName) {
    /*
    Returns a reference to the object given by objectName
    */
    for (int i = 0; i < interactableObjects.size(); i++) {
        if (objectName == interactableObjects.at(i)->getName())
            return interactableObjects.at(i);
    }
}

std::vector<GenericEnemy*>::iterator Location::enemyPosition(GenericEnemy *enemy) {
    /* 
    Returns a vector iterator, with value corresponding to the index of the enemies vector
    if enemy is in the vector. Otherwise, returns the size of enemies.
    */
    return std::find(enemies.begin(), enemies.end(), enemy);
}

void Location::addEnemy(GenericEnemy *enemy) {
    /*
    Adds new enemy to enemies if it is not already in it
    */
    if (enemyPosition(enemy) == enemies.end())
        enemies.push_back(enemy);
}

void Location::removeEnemy(GenericEnemy *enemy) {
    /*
    Removes enemy from enemies if it is in it
    Otherwise, does nothing
    */
    std::vector<GenericEnemy*>::iterator position = enemyPosition(enemy);
    if (position != enemies.end())
        enemies.erase(position);
}

bool Location::checkStringInEnemies(std::string enemyName) {
    /*
    Checks if the string corresponds to an enemy name in enemies
    */
    for (int i = 0; i < enemies.size(); i++) {
        if (enemyName == enemies.at(i)->getName())
            return true;
    }
    return false;
}

GenericEnemy *Location::getEnemyFromString(std::string enemyName) {
    /*
    Returns a reference to the enemy given by enemyName
    */
    for (int i = 0; i < enemies.size(); i++) {
        if (enemyName == enemies.at(i)->getName())
            return enemies.at(i);
    }
}

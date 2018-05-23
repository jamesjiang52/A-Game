#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "GenericEnemyClass.hpp"
#include "LocationClass.hpp"

class GenericEnemy;

class Player {
    private: 
        const int MAX_ARMOR = 100; // armor is capped at this value
        const double MAX_ARMOR_DMG_REDUCTION = 0.8; // between 0 and 1, higher is better
        int startingHealth;
        int currentHealth;
        int armor;
        int attackDamage;
        int speed;
        Location *location;
        std::string name;

    public:
        Player(std::string name, int startingHealth, int armor, int attackDamage, Location *startingLocation);
        int getCurrentHealth() const;
        void loseHealth(int amount);
        void attack(GenericEnemy *enemy);
        Location *getLocation() const;
        void setLocation(Location *location);
    };

#endif

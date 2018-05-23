#ifndef GENERIC_ENEMY_H
#define GENERIC_ENEMY_H

#include <string>
#include "PlayerClass.hpp"
#include "LocationClass.hpp"

class Player;

class GenericEnemy {
    protected:
        const int MAX_ARMOR = 100;  // armor is capped at this value
        const double MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
        int startingHealth;
        int currentHealth;
        int armor;
        int attackDamage;
        Location *location;
        std::string name;
        
    public:
        GenericEnemy(std::string name, int startingHealth, int armor, int attackDamage, Location *startingLocation);
        int getCurrentHealth() const;
        void loseHealth(int amount);
        void attack(Player *player);
        Location *getLocation() const;
        void setLocation(Location *location);
};

#endif

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "GenericEnemyClass.hpp"
#include "LocationClass.hpp"

class GenericEnemy;

class Player {
    private: 
        const int MAX_ARMOR = 100;  // armor is capped at this value
        const double MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
        int startingHealth;
        int currentHealth;
        int armor;
        int attackDamage;
        int speed;
        Location *location;
        std::string name;
		std::vector<Item*> invontory;

    public:
        Player(std::string name, int startingHealth, int armor, int attackDamage, Location *startingLocation);
        int getCurrentHealth() const;
        void loseHealth(int amount);
        void attack(GenericEnemy *enemy);
        Location *getLocation() const;
        void setLocation(Location *location);
		std::vector<Item*>::iterator getInvontory(Item object) const;
		void addToInvontory(Item object);
		void removeFromInvontory(Item object);
};

#endif

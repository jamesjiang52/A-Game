#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <algorithm>
#include "GenericEnemyClass.hpp"
#include "LocationClass.hpp"
#include "InteractableObjectClass.hpp"
#include "WeaponClass.hpp"
#include "ArmorClass.hpp"

class GenericEnemy;

class Player {
    private: 
        const int MAX_ARMOR = 100;  // armor is capped at this value
        const double MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
        int startingHealth;
        int currentHealth;
        Weapon *weapon;
        Armor *armor;
        int speed;
        Location *location;
        std::string name;
        std::vector<InteractableObject*> inventory;

    public:
        Player(std::string name, int startingHealth, Location *startingLocation);
        int getCurrentHealth() const;
        void loseHealth(int amount);
        void attack(GenericEnemy *enemy);
        Location *getLocation() const;
        void setLocation(Location *location);
        std::vector<InteractableObject*> getInventory() const;
        std::vector<InteractableObject*>::iterator objectPosition(InteractableObject *object);
        void addToInventory(InteractableObject *object);
        void removeFromInventory(InteractableObject *object);
        Weapon *getWeapon() const;
        void setWeapon(Weapon *weapon);
        Armor *getArmor() const;
        void setArmor(Armor *armor);
};

#endif

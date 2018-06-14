#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "InteractableObjectClass.hpp"
#include "LocationClass.hpp"
#include "WeaponClass.hpp"
#include "ArmorClass.hpp"
#include "GenericEnemyClass.hpp"

class GenericEnemy;
class Armor;
class Weapon;

class Player {
    private: 
        const double MAX_ARMOR = 100;  // armor is capped at this value
        const double MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
        int startingHealth;
        int currentHealth;
        Weapon *weapon;
        Armor *armor;
        int speed;
        Location *location;
        std::string name;
        std::vector<InteractableObject*> inventory;
        std::vector<InteractableObject*>::iterator objectPosition(InteractableObject *object);

    public:
        Player(std::string name, int startingHealth);
        std::string getName() const;
        int getStartingHealth() const;
        int getCurrentHealth() const;
        void loseHealth(int amount);
        void gainHealth(int amount);
        void attack(GenericEnemy *enemy);
        Location *getLocation() const;
        void setLocation(Location *location);
        std::vector<InteractableObject*> getInventory() const;
        bool checkStringInInventory(std::string objectName);
        InteractableObject *getObjectFromString(std::string objectName);
        void addToInventory(InteractableObject *object);
        void removeFromInventory(InteractableObject *object);
        Weapon *getWeapon() const;
        void setWeapon(Weapon *weapon);
        Armor *getArmor() const;
        void setArmor(Armor *armor);
};

#endif

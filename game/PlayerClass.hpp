#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Parameters.hpp"
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
        int startingHealth;
        int currentHealth;
        Weapon *weapon;
        Armor *armor;
        Shield *shield;
        Location *location;
        std::string name;
        std::vector<InteractableObject*> inventory;
        std::vector<InteractableObject*>::iterator objectPosition(InteractableObject *object);
        bool canUseShield;

    public:
        Player(std::string name, int startingHealth);
        std::string getName() const;
        int getStartingHealth() const;
        int getCurrentHealth() const;
        int getTotalEncumbrance() const;
        bool isOverEncumbered() const;
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
        Shield *getShield() const;
        void setShield(Shield *shield);
        bool canUseShield() const;
};

#endif

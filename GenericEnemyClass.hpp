#ifndef GENERIC_ENEMY_H
#define GENERIC_ENEMY_H

#include <string>
#include "WeaponClass.hpp"
#include "ArmorClass.hpp"
#include "PlayerClass.hpp"

class Player;
class Weapon;
class Armor;

class GenericEnemy {
    protected:
        const double MAX_ARMOR = 100;  // armor is capped at this value
        const double MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
        int startingHealth;
        int currentHealth;
        Weapon *weapon;
        Armor *armor;
        std::string name;
        
    public:
        GenericEnemy(std::string name, int startingHealth);
        std::string getName() const;
        int getStartingHealth() const;
        int getCurrentHealth() const;
        void loseHealth(int amount);
        void attack(Player *player);
        Weapon *getWeapon() const;
        void setWeapon(Weapon *weapon);
        Armor *getArmor() const;
        void setArmor(Armor *armor);
};

#endif

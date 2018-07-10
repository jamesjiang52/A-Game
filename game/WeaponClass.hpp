#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <string>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class Weapon: public InteractableObject {
    private:
        int damage;
        int playerStaggerPercentIncrease;
        int enemyStaggerPercentIncrease;
        int enemyArmorReductionPercent;
        int playerHealthBleed;
        int enemyHealthBleed;
        bool twoHanded;
    
    public:
        Weapon(
            std::string name,
            std::string description,
            int encumbrance,
            int damage,
            int playerStaggerPercentIncrease,
            int enemyStaggerPercentIncrease,
            int enemyArmorReductionPercent,
            int playerHealthBleed,
            int enemyHealthBleed,
            bool isTwoHanded
        );
        int getDamage() const;
        int getPlayerStaggerPercentIncrease() const;
        int getEnemyStaggerPercentIncrease() const;
        bool isTwoHanded() const;
        void use(Player *player);  // equips weapon
};

#endif

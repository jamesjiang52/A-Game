#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>
#include <string>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"
#include "ActiveEffectClass.hpp"

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
        ActiveEffect *activeEffect;
    
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
            bool isTwoHanded,
            ActiveEffect *activeEffect
        );
        ~Weapon();
        int getDamage() const;
        int getPlayerStaggerPercentIncrease() const;
        int getEnemyStaggerPercentIncrease() const;
        int getEnemyArmorReductionPercent() const;
        int getPlayerHealthBleed() const;
        int getEnemyHealthBleed() const;
        std::string getStatString() const;
        bool isTwoHanded() const;
        ActiveEffect *getActiveEffect() const;
        void use(Player *player);  // equips weapon
};

#endif

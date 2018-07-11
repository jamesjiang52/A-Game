#ifndef ARMOR_H
#define ARMOR_H

#include <iostream>
#include <string>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class Armor: public InteractableObject {
    private:
        int armor;
        int playerStaggerPercentIncrease;
        int enemyStaggerPercentIncrease;
    
    public:
        Armor(std::string name, std::string description, int encumbrance, int armor, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease);
        int getArmor() const;
        int getPlayerStaggerPercentIncrease() const;
        int getEnemyStaggerPercentIncrease() const;
        std::string getStatString() const;
        void use(Player *player);  // equips armor
};

#endif

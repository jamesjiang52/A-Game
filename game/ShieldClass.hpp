#ifndef SHIELD_H
#define SHIELD_H

#include <iostream>
#include <string>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class Shield: public InteractableObject {
    private:
        int armor;
        int playerStaggerPercentIncrease;
        int enemyStaggerPercentIncrease;
    
    public:
        Shield(std::string name, std::string description, int encumbrance, int armor, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease);
        int getArmor() const;
        int getPlayerStaggerPercentIncrease() const;
        int getEnemyStaggerPercentIncrease() const;
        std::string getStatString() const;
        void use(Player *player);  // equips shield
};

#endif

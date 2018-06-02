#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class Weapon: public InteractableObject {
    private:
        int damage;
    
    public:
        Weapon(std::string name, std::string description, int damage);
        int getDamage();
        void use(Player *player);  // equips weapon
};

#endif

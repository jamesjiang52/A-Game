#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include "InteractableObjectClass.hpp"

class Weapon: public InteractableObject {
    private:
        int damage;
    
    public:
        Weapon(std::string name, std::string useMessage, int damage);
        int getDamage();
};

#endif

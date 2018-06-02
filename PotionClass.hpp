#ifndef POTION_H
#define POTION_H

#include <string>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class Potion: public InteractableObject {
    private:
        int healAmount;
    
    public:
        Potion(std::string name, std::string description, int healAmount);
        int getHealAmount();
        void use(Player *player);  // use potion
};

#endif

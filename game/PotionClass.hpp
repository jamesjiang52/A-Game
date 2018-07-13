#ifndef POTION_H
#define POTION_H

#include <string>
#include <functional>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class Potion: public InteractableObject {
    private:
        std::function<void(Player *player)> useFunction;
    
    public:
        Potion(std::string name, std::string description, int encumbrance, std::function<void(Player *player)> useFunction);
        void use(Player *player);  // use potion
};

#endif

#ifndef POTION_H
#define POTION_H

#include <string>
#include <functional>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class Potion: public InteractableObject {
    private:
        function<void()> useFunction(Player *player);
    
    public:
        Potion(std::string name, std::string description, int encumbrance, const function<void()>& useFunction(Player *player));
        void use(Player *player);  // use potion
};

#endif

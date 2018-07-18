#ifndef MISC_OBJECT_H
#define MISC_OBJECT_H

#include <string>
#include <functional>
#include "InteractableObjectClass.hpp"
#include "PlayerClass.hpp"

class Player;

class MiscObject: public InteractableObject {
    private:
        std::function<void(Player *player)> useFunction;
    
    public:
        MiscObject(std::string name, std::string description, int encumbrance, std::function<void(Player *player)> useFunction);
        void use(Player *player);  // use potion
};

#endif

#ifndef ARMOR_H
#define ARMOR_H

#include <string>
#include "InteractableObjectClass.hpp"

class Armor: public InteractableObject {
    private:
        int armor;
    
    public:
        Armor(std::string name, std::string useMessage, int armor);
        int getArmor();
};

#endif

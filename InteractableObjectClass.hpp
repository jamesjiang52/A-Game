#ifndef INTERACTABLE_OBJECT_H
#define INTERACTABLE_OBJECT_H

#include <string>

class Player;

// This class is used for basically everything that the user can interact with (with the exception of 
// location directions). These include weapons, armor, potions, and whatever else might be conceived of
// later on. All these things will have their own class that inherits attributes from this class.
class InteractableObject {
    protected:
        std::string name;
        std::string description;  // shown when player looks at the object
    
    public:
        InteractableObject(std::string name, std::string description);
        bool operator==(const InteractableObject &object);
        std::string getName() const;
        std::string getDescription() const;
        virtual void use(Player *player);
};

#endif

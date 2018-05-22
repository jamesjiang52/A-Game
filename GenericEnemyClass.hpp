#ifndef GENERIC_ENEMY_H
#define GENERIC_ENEMY_H

#include <string>
#include "PlayerClass.hpp"
#include "LocationClass.hpp"

class GenericEnemy {
    private:
        const int MAX_ARMOR = 100;  // armor is capped at this value
        const double MAX_ARMOR_DMG_REDUCTION = 0.8;  // between 0 and 1, higher is better
        int startingHealth;
        int currentHealth;
        int armor;
        int attackDamage;
        Location location;
        std::string name;
        
    public:
        int getCurrentHealth();
        void loseHealth(int amount);
        void attack(Player* player);
}

GenericEnemy::GenericEnemy(std::string name, int startingHealth, int armor, int attackDamage, Location startingLocation) {
    this->name = name;
    this->startingHealth = startingHealth;
    this->currentHealth = startingHealth;
    this->attackDamage = attackDamage;

    if (armor < MAX_ARMOR) 
        this->armor = armor;
    else
        this->armor = MAX_ARMOR;
    
    this->location = startingLocation;
}

int GenericEnemy::getCurrentHealth() {
    return currentHealth;
}

void GenericEnemy::loseHealth(int amount) {
    double damageReductionFraction = armor/MAX_ARMOR*MAX_ARMOR_DMG_REDUCTION;
    // ^ this value is 0 at armor=0 and MAX_ARMOR_DMG_REDUCTION at armor=MAX_ARMOR

    int loseHealthAmount = amount*(1 - damageReductionFraction);
    
    if (currentHealth - loseHealthAmount > 0)
        currentHealth -= loseHealthAmount;
    else {  // enemy dies
        currentHealth = 0;
        // probably do something else as well
    }
}

void GenericEnemy::attack(Player *player) {
    player->loseHealth(attackDamage);
}

Location GenericEnemy::getLocation() {
    return location;
}

void GenericEnemy::setLocation(Location location) {
    this->location = location;
}

#endif

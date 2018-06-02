#include "GenericEnemyClass.hpp"

GenericEnemy::GenericEnemy(std::string name, int startingHealth, Location *startingLocation) {
    this->name = name;
    this->startingHealth = startingHealth;
    this->currentHealth = startingHealth;

    this->weapon = new Weapon("", "", "", 0);  // name, useMessage, damage
    this->armor = new Armor("", "", "", 0);  // name, useMessage, armor
    
    this->location = startingLocation;
}

int GenericEnemy::getStartingHealth() const {
    return startingHealth;
}

int GenericEnemy::getCurrentHealth() const {
    return currentHealth;
}

void GenericEnemy::loseHealth(int amount) {
    double damageReductionFraction = armor->getArmor()/MAX_ARMOR*MAX_ARMOR_DMG_REDUCTION;
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
    player->loseHealth(weapon->getDamage());
}

Location *GenericEnemy::getLocation() const {
    return location;
}

void GenericEnemy::setLocation(Location *location) {
    this->location = location;
}

Weapon *GenericEnemy::getWeapon() const {
    return weapon;
}

void GenericEnemy::setWeapon(Weapon *weapon) {
    this->weapon = weapon;
}

Armor *GenericEnemy::getArmor() const {
    return armor;
}

void GenericEnemy::setArmor(Armor *armor) {
    this->armor = armor;
}

#include "PlayerClass.hpp"

Player::Player(std::string name, int startingHealth, Location *startingLocation) {
    this->name = name;
    this->startingHealth = startingHealth;
    this->currentHealth = startingHealth;

    this->weapon = new Weapon(NULL, NULL, 0);  // name, useMessage, damage
    this->armor = new Armor(NULL, NULL, 0);  // name, useMessage, armor

    this->location = startingLocation;
}

int Player::getCurrentHealth() const {
    return currentHealth;
}

void Player::loseHealth(int amount) {
    double damageReductionFraction = armor->getArmor()/MAX_ARMOR*MAX_ARMOR_DMG_REDUCTION;
    // ^ this value is 0 at armor=0 and MAX_ARMOR_DMG_REDUCTION at armor=MAX_ARMOR

    int loseHealthAmount = amount*(1 - damageReductionFraction);

    if (currentHealth - loseHealthAmount > 0)
        currentHealth -= loseHealthAmount;
    else {  // player loses
        currentHealth = 0;
        // probably do something else as well (i.e. game over)
    }
}

void Player::attack(GenericEnemy *enemy) {
    enemy->loseHealth(weapon->getDamage());
}

Location *Player::getLocation() const {
    return location;
}

void Player::setLocation(Location *location) {
    this->location = location;
}

std::vector<InteractableObject*> Player::getInventory() const {
    return inventory;
}

std::vector<InteractableObject*>::iterator Player::objectPosition(InteractableObject *object) {
    /* 
    Returns a vector iterator, with value corresponding to the index of the inventory vector
    if object is in the vector. Otherwise, returns the size of the inventory.
    */
    return std::find(inventory.begin(), inventory.end(), object);
}

void Player::addToInventory(InteractableObject *object) {
    /*
    Adds new object to inventory if it is not already in it
    */
    if (objectPosition(object) == inventory.end())
        inventory.push_back(object);
}

void Player::removeFromInventory(InteractableObject *object) {
    /*
    Removes object from inventory if it is in it
    Otherwise, does nothing
    */
    std::vector<InteractableObject*>::iterator position = objectPosition(object);
    if (position != inventory.end())
        inventory.erase(position);
}

Weapon *Player::getWeapon() const {
    return weapon;
}

void Player::setWeapon(Weapon *weapon) {
    this->weapon = weapon;
}

Armor *Player::getArmor() const {
    return armor;
}

void Player::setArmor(Armor *armor) {
    this->armor = armor;
}

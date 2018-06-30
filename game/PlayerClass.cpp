#include "PlayerClass.hpp"

Player::Player(std::string name, int startingHealth) {
    this->name = name;
    this->startingHealth = startingHealth;
    this->currentHealth = startingHealth;

    this->weapon = new Weapon("iron sword", "", 3, 10);  // name, description, encumbrance, damage
    this->armor = new Armor("street clothes", "", 4, 1);  // name, description, encumbrance, armor
}

std::string Player::getName() const {
    return name;
}

int Player::getStartingHealth() const {
    return startingHealth;
}

int Player::getCurrentHealth() const {
    return currentHealth;
}

int Player::getTotalEncumbrance() const {
    int sum = 0;
    for (int i = 0; i < inventory.size(); i++) {
        InteractableObject *object = inventory.at(i);
        sum += object->getEncumbrance();
    }
    return sum;
}

bool Player::isOverEncumbered() const {
    return(getTotalEncumbrance() > MAX_ENCUMBRANCE);
}

void Player::loseHealth(int amount) {
    double damageReductionFraction = std::min(armor->getArmor(), MAX_ARMOR)*MAX_ARMOR_DMG_REDUCTION/MAX_ARMOR;
    // ^ this value is 0 at armor=0 and MAX_ARMOR_DMG_REDUCTION at armor=MAX_ARMOR

    int loseHealthAmount = amount*(1 - damageReductionFraction);

    if (currentHealth - loseHealthAmount > 0)
        currentHealth -= loseHealthAmount;
    else {  // player loses
        currentHealth = 0;
        // probably do something else as well (i.e. game over)
    }
}

void Player::gainHealth(int amount) {
    if (currentHealth + amount < startingHealth)
        currentHealth += amount;
    else
        currentHealth = startingHealth;
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
    if object is in the vector. Otherwise, returns the iterator with the sizes of the inventory.
    */
    return std::find(inventory.begin(), inventory.end(), object);
}

bool Player::checkStringInInventory(std::string objectName) {
    /*
    Checks if the string corresponds to an object name in inventory
    */
    for (int i = 0; i < inventory.size(); i++) {
        if (objectName == inventory.at(i)->getName())
            return true;
    }
    return false;
}

InteractableObject *Player::getObjectFromString(std::string objectName) {
    /*
    Returns a reference to the object given by objectName
    */
    for (int i = 0; i < inventory.size(); i++) {
        if (objectName == inventory.at(i)->getName())
            return inventory.at(i);
    }
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

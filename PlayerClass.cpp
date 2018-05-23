#include "PlayerClass.hpp"

Player::Player(std::string name, int startingHealth, int armor, int attackDamage, Location *startingLocation) {
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

int Player::getCurrentHealth() const {
    return currentHealth;
}

void Player::loseHealth(int amount) {
    double damageReductionFraction = armor/MAX_ARMOR*MAX_ARMOR_DMG_REDUCTION;
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
    enemy->loseHealth(attackDamage);
}

Location *Player::getLocation() const {
    return location;
}

void Player::setLocation(Location *location) {
    this->location = location;
}

#include "PlayerClass.hpp"

Player::Player(std::string name, int startingHealth) {
    this->name = name;
    this->startingHealth = startingHealth;
    this->currentHealth = startingHealth;

    this->weapon = NULL;
    this->armor = NULL;
    this->shield = NULL;
    
    oneHanded = true;
    questStage = 0;
}

Player::Player(const Player &player) {
    name = player.name;
    startingHealth = player.startingHealth;
    currentHealth = player.currentHealth;
    weapon = player.weapon;
    armor = player.armor;
    shield = player.shield;
    location = player.location;
    oneHanded = player.oneHanded;
    
    for (int i = 0; i < player.inventory.size(); i++) addToInventory(player.inventory.at(i));
    for (int i = 0; i < player.activeEffects.size(); i++) addToActiveEffects(player.activeEffects.at(i));
}

Player& Player::operator=(const Player &player) {
    name = player.name;
    startingHealth = player.startingHealth;
    currentHealth = player.currentHealth;
    weapon = player.weapon;
    armor = player.armor;
    shield = player.shield;
    location = player.location;
    oneHanded = player.oneHanded;
    
    inventory.clear();
    activeEffects.clear();
    for (int i = 0; i < player.inventory.size(); i++) addToInventory(player.inventory.at(i));
    for (int i = 0; i < player.activeEffects.size(); i++) addToActiveEffects(player.activeEffects.at(i));
}

Player::~Player() {
    delete weapon;
    delete armor;
    delete shield;
    for (int i = 0; i < inventory.size(); i++) delete inventory.at(i);
    for (int i = 0; i < activeEffects.size(); i++) delete activeEffects.at(i);
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

void Player::loseHealth(int amount, int armorReductionPercent) {
    float damageReductionFraction;
    if (shield) damageReductionFraction = std::min(armor->getArmor() + shield->getArmor(), MAX_ARMOR)*MAX_ARMOR_DMG_REDUCTION*(1 - armorReductionPercent/100.0)/MAX_ARMOR;
    // ^ this value is 0 at armor=0 and MAX_ARMOR_DMG_REDUCTION at armor=MAX_ARMOR
    else damageReductionFraction = std::min(armor->getArmor(), MAX_ARMOR)*MAX_ARMOR_DMG_REDUCTION*(1 - armorReductionPercent/100.0)/MAX_ARMOR;

    int loseHealthAmount = std::max((int)(amount*(1 - damageReductionFraction)), 0);  // don't let damage go below 0

    if (currentHealth - loseHealthAmount > 0)
        currentHealth -= loseHealthAmount;
    else {  // player loses
        currentHealth = 0;
    }
}

void Player::gainHealth(int amount) {
    if (currentHealth + amount < startingHealth)
        currentHealth += amount;
    else if (currentHealth + amount <= 0)
        currentHealth = 0;
    else
        currentHealth = startingHealth;
}

void Player::attack(GenericEnemy *enemy) {
    enemy->loseHealth(weapon->getDamage());
}

int getQuestStage() const {
    return questStage;
}

void setQuestStage(int stage) {
    this->questStage = stage;
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
    if object is in the vector. Otherwise, returns the iterator with the size of the inventory.
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
    Adds new object to inventory
    */
    if (object)
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

std::vector<ActiveEffect*> Player::getActiveEffects() const {
    return activeEffects;
}

std::vector<ActiveEffect*>::iterator Player::effectPosition(ActiveEffect *activeEffect) {
    /* 
    Returns a vector iterator, with value corresponding to the index of the active effect vector
    if effect is in the vector. Otherwise, returns the iterator with the size of the active effect vector.
    */
    return std::find(activeEffects.begin(), activeEffects.end(), activeEffect);
}

void Player::addToActiveEffects(ActiveEffect *activeEffect, bool allowDuplicate) {
    /*
    Adds new effect to active effects
    */
    if (activeEffect) {
        if (allowDuplicate)
            activeEffects.push_back(activeEffect);
        else {
            std::vector<ActiveEffect*>::iterator position = effectPosition(activeEffect);
            if (position != activeEffects.end())
                activeEffects.push_back(activeEffect);
        }
    }
}

void Player::removeFromActiveEffects(ActiveEffect *activeEffect) {
    /*
    Removes effect from active effects if it is in it
    Otherwise, does nothing
    */
    std::vector<ActiveEffect*>::iterator position = effectPosition(activeEffect);
    if (position != activeEffects.end())
        activeEffects.erase(position);  // this also automatically frees the memory allocated for the object
}

Weapon *Player::getWeapon() const {
    return weapon;
}

void Player::setWeapon(Weapon *weapon) {
    this->weapon = weapon;
    
    if (weapon->isTwoHanded()) {
        oneHanded = false;
    } else {
        oneHanded = true;
    }
}

Armor *Player::getArmor() const {
    return armor;
}

void Player::setArmor(Armor *armor) {
    this->armor = armor;
}

Armor *Player::createStreetClothes() {
    Armor *armor = new Armor(
        "street clothes",
        "The uniform of a lowly soldier consists of a navy coat, black pants, and sturdy boots.\n",
        1, 1, 0, 0,
        NULL
    );
    armor->appendToDescription(armor->getStatString());
    return armor;
}

void Player::setArmorToStreetClothes() {
    InteractableObject *oldStreetClothes = getObjectFromString("street clothes");
    removeFromInventory(oldStreetClothes);
    delete oldStreetClothes;
    
    Armor *newStreetClothes = createStreetClothes();
    addToInventory(newStreetClothes);
    this->armor = newStreetClothes;
}

Shield *Player::getShield() const {
    return shield;
}

void Player::setShield(Shield *shield) {
    this->shield = shield;
}

bool Player::canUseShield() const {
    return oneHanded;
}

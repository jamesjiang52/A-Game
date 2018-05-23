#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "LocationClass.hpp"
#include "GenericEnemyClass.hpp"

class Player {
	private: 
		const int MAX_ARMOR = 100; // armor is capped at this value
		const double MAX_ARMOR_DMG_REDUCTION = 0.8; // between 0 and 1, higher is better
		int startingHealth;
		int currentHealth;
		int attackDamage;
		int armor;
		int speed;
		Location location;
		std::string name;

	public:
		int getCurrentHealth();
		void loseHealth(int amount);
		void attack(GenericEnemy *enemy);
		Location getLocation();
		void setLocation(Location location);

};

Player::Player(std::string name, int startingHealth, int armor, int attackDamage, Location startingLocation) {
	this->name = name;
	this->startingHealth = startingHealth;
	this->currentHealth = startingHealth;
	this->attackDamage = attackDamage;

	if (armor < MAX_ARMOR) {
		this->armor = armor;
	}
	else {
		this->armor = MAX_ARMOR;
	}

	this->location = startingLocation;
}

int Player::getCurrentHealth() {
	return currentHealth;
}

void Player::loseHealth(int amount) {
	double damageReductionFraction = armor / MAX_ARMOR * MAX_ARMOR_DMG_REDUCTION;
	// ^ this value is 0 at armor=0 and MAX_ARMOR_DMG_REDUCTION at armor=MAX_ARMOR

	int loseHealthAmount = amount * (1 - damageReductionFraction);

	if (currentHealth - loseHealthAmount > 0)
		currentHealth -= loseHealthAmount;
	else {  // Player loses
		currentHealth = 0;
		// probably do something else as well (ie: game over)
	}
}

void Player::attack(GenericEnemy *enemy) {
	enemy->loseHealth(attackDamage);
}

Location Player::getLocation() {
	return location;
}

void Player::setLocation(Location location) {
	this->location = location;
}

#endif
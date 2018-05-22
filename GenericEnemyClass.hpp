#ifndef GENERIC_ENEMY_H
#define GENERIC_ENEMY_H

class GenericEnemy {
    private:
        const int MAX_ARMOR = 100;
        const double MAX_ARMOR_DMG_REDUCTION = 0.8;
        int startingHealth;
        int currentHealth;
        int armor;
        int attackDamage;
        
    public:
        int getCurrentHealth();
        void loseHealth(int);
        void attack(Player*);
}

GenericEnemy::GenericEnemy (startingHealth, armor, attackDamage) {
    this->startingHealth = startingHealth;
    this->currentHealth = startingHealth;
    this->attackDamage = attackDamage;

    if (armor < MAX_ARMOR) 
        this->armor = armor;
    else
        this->armor = MAX_ARMOR;
}

int GenericEnemy::getCurrentHealth() {
    return this->currentHealth;
}

void GenericEnemy::loseHealth(int amount) {
    double damageReductionFraction = armor/MAX_ARMOR*MAX_ARMOR_DMG_REDUCTION;
    // ^ this value is 0 at armor=0 and MAX_ARMOR_DMG_REDUCTION at armor=MAX_ARMOR

    int loseHealthAmount = amount*(1 - damageReductionFraction);
    
    if (this->currentHealth - loseHealthAmount > 0)
        this->currentHealth -= loseHealthAmount;
    else {  // enemy dies
        this->currentHealth = 0;
        // probably do something else as well
    }
}

void GenericEnemy::attack(Player *player) {
    player->loseHealth(this->attackDamage);
}

#endif

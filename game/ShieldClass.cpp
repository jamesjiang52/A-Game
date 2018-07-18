#include "ShieldClass.hpp"

Shield::Shield(std::string name, std::string description, int encumbrance, int armor, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease, ActiveEffect *activeEffect): InteractableObject(name, description, encumbrance) {
    this->armor = armor;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
    this->activeEffect = activeEffect;
}

Shield::~Shield() {
    delete activeEffect;
}

int Shield::getArmor() const {
    return armor;
}

int Shield::getPlayerStaggerPercentIncrease() const {
    return playerStaggerPercentIncrease;
}

int Shield::getEnemyStaggerPercentIncrease() const {
    return enemyStaggerPercentIncrease;
}

std::string Shield::getStatString() const {
    /*
    Returns a string with shield stats. Each stat, beginning with armor, is separated by a newline and begins
    with four spaces. Leading and trailing newlines to the entire statstring are absent.
    */
    std::string stats = "    ";
    
    stats += std::to_string(armor) + " armor";
    if (playerStaggerPercentIncrease > 0)
        stats += "\n    +" + std::to_string(playerStaggerPercentIncrease) + "% player stagger";
    else if (playerStaggerPercentIncrease < 0)
        stats += "\n    -" + std::to_string(playerStaggerPercentIncrease) + "% player stagger";
    if (enemyStaggerPercentIncrease > 0)
        stats += "\n    +" + std::to_string(enemyStaggerPercentIncrease) + "% enemy stagger";
    else if (enemyStaggerPercentIncrease < 0)
        stats += "\n    -" + std::to_string(enemyStaggerPercentIncrease) + "% enemy stagger";
    
    return stats;
}

ActiveEffect *Shield::getActiveEffect() const {
    return activeEffect;
}

void Shield::use(Player *player) {
    if (!player->canUseShield()) {
        std::cout << "The " << player->getWeapon()->getName() << " requires both of my hands to wield. I must switch to something different if I want to use a shield.\n\n";
    } else if (!player->getShield()) {  // no shield currently equipped
        std::cout << "I equip the " << name << " in my off hand.\n\n";
        player->addToActiveEffects(activeEffect);
        player->setShield(this);
    } else if (player->getShield() != this) {
        player->removeFromActiveEffects(player->getShield()->getActiveEffect());
        player->addToActiveEffects(activeEffect);
        std::cout << "I put the " << player->getShield()->getName() << " back into my knapsack and equip the " << name << " in my off hand.\n\n";
    } else {  // shield is already equipped
        std::cout << "I already have the " << name << " equipped.\n\n";
    }
}

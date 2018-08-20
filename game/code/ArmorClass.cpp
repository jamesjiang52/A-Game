#include "ArmorClass.hpp"

Armor::Armor(std::string name, std::string description, int encumbrance, int armor, int playerStaggerPercentIncrease, int enemyStaggerPercentIncrease, ActiveEffect *activeEffect): InteractableObject(name, description, encumbrance) {
    this->armor = armor;
    this->playerStaggerPercentIncrease = playerStaggerPercentIncrease;
    this->enemyStaggerPercentIncrease = enemyStaggerPercentIncrease;
    this->activeEffect = activeEffect;
}

Armor::~Armor() {
    delete activeEffect;
}

int Armor::getArmor() const {
    return armor;
}

int Armor::getPlayerStaggerPercentIncrease() const {
    return playerStaggerPercentIncrease;
}

int Armor::getEnemyStaggerPercentIncrease() const {
    return enemyStaggerPercentIncrease;
}

std::string Armor::getStatString() const {
    /*
    Returns a string with armor stats. Each stat, beginning with armor, is separated by a newline and begins
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

ActiveEffect *Armor::getActiveEffect() const {
    return activeEffect;
}

void Armor::use(Player *player) {
    if (name == "street clothes") {
        if (player->getArmor() == this) {
            std::cout << "I'm already wearing the street clothes.\n\n";
        } else {
            std::cout << "I'm already wearing the street clothes inside my " << player->getArmor()->getName() << ".\n\n";
        }
    } else if (player->getArmor() != this) {
        std::cout << "I take off the " << player->getArmor()->getName() << " and put on the " << name << ".\n\n";
        player->removeFromActiveEffects(player->getArmor()->getActiveEffect());
        player->addToActiveEffects(activeEffect);
        player->setArmor(this);
    } else {
        std::cout << "I'm already wearing the " << name << ".\n\n";
    }
}

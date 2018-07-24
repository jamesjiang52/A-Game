#include "Potions.hpp"

void wholeWheatBreadUseFunction(Player *player) {
    player->gainHealth(20);
    std::cout << "Feels healthier than white bread... I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
}

Potion *createWholeWheatBread() {
    Potion *potion = new Potion(
        "whole wheat bread",
        "This is only available from my favourite bakery (+20 health).",
        1,
        wholeWheatBreadUseFunction
    );
    return potion;
}

void whiteBreadUseFunction(Player *player) {
    player->gainHealth(15);
    std::cout << "Tastes better than whole wheat bread... I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
    ActiveEffect *effect = new ActiveEffect("health", 5, 5, 0);  // +5 health after 5 combat turns
    player->addToActiveEffects(effect);
}

Potion *createWhiteBread() {
    Potion *potion = new Potion(
        "white bread",
        "This is only available from my favourite bakery (+15 health).",
        1,
        whiteBreadUseFunction
    );
    return potion;
}

void bottleOfMeadUseFunction(Player *player) {
    player->gainHealth(15);
    std::cout << "I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health). What was hurting again?\n\n";
}

Potion *createBottleOfMead() {
    Potion *potion = new Potion(
        "bottle of mead",
        "Nothing soothes pain like a dash of mead - a single gulp and all is forgotten (+15 health).",
        1,
        bottleOfMeadUseFunction
    );
    return potion;
}

void squareOfChocolateUseFunction(Player *player) {
    player->gainHealth(10);
    std::cout << "I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health). Delicious!\n\n";
}

Potion *createSquareOfChocolate() {
    Potion *potion = new Potion(
        "square of chocolate",
        "There's nothing like the gooey wonderfulness of this brown-coloured delicacy (+10 health).",
        1,
        squareOfChocolateUseFunction
    );
    return potion;
}

void appleUseFunction(Player *player) {
    player->gainHealth(10);
    std::cout << "I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health). Kept the doctor away for another day!\n\n";
}

Potion *createApple() {
    Potion *potion = new Potion(
        "apple",
        "An apple a day keeps the doctor away (+10 health).",
        1,
        appleUseFunction
    );
    return potion;
}

void bananaUseFunction(Player *player) {
    player->gainHealth(10);
    std::cout << "I too once was young... I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
}

Potion *createBanana() {
    Potion *potion = new Potion(
        "banana",
        "This banana was once young and green (+10 health).",
        1,
        bananaUseFunction
    );
    return potion;
}

void durrianUseFunction(Player *player) {
    int choices[2] = {-5, 10};
    std::srand(std::time(nullptr));
    int choice = choices[std::rand() % 2];
    player->gainHealth(choice);
    
    if (choice > 0) {
        std::cout << "Even heaven pales in comparison; I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
    } else if (!player->getCurrentHealth()) { // player (almost) dies from eating durrian
        player->gainHealth(1);
        std::cout << "The smell... it is too much. I am put on the brink of death (" << "1/" << player->getStartingHealth() << " health).\n\n";
    } else {
        std::cout << "The smell... it is too much. I can feel my lifeforce weakening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
    } 
}

Potion *createDurrian() {
    Potion *potion = new Potion(
        "durrian",
        "These taste like heaven... but smell like hell (random between +10 health and -5 health).",
        1,
        durrianUseFunction
    );
    return potion;
}

void lemonUseFunction(Player *player) {
    player->gainHealth(5);
    std::cout << "I can't make lemonade, so I guess this will have to do. I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
    ActiveEffect *effect = new ActiveEffect("health", 10, 5, 0);  // +10 health after 5 combat turns
    player->addToActiveEffects(effect);
}

Potion *createLemon() {
    Potion *potion = new Potion(
        "lemon",
        "Only when you have eaten a lemon do you appreciate what \"sweet\" is (+5 health).",
        1,
        lemonUseFunction
    );
    return potion;
}

void peachUseFunction(Player *player) {
    player->gainHealth(10);
    std::cout << "Life is short and winter is coming. I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
}

Potion *createPeach() {
    Potion *potion = new Potion(
        "peach",
        "A man should never refuse to taste a peach. He may never get the chance again (+10 health).",
        1,
        peachUseFunction
    );
    return potion;
}

void ambrosiaUseFunction(Player *player) {
    int choices[3] = {15, 25, -20};
    std::srand(std::time(nullptr));
    int choice = choices[std::rand() % 3];
    player->gainHealth(choice);
    
    std::cout << "Have I been blessed or cursed? ";
    if (choice > 0) {
        std::cout << "I can feel my lifeforce strengthening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
    } else if (!player->getCurrentHealth()) { // player (almost) dies from eating ambrosia
        player->gainHealth(1);
        std::cout << "The ambrosia puts me on the brink of death (" << "1/" << player->getStartingHealth() << " health).\n\n"; 
    } else {
        std::cout << "I can feel my lifeforce weakening (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n"; 
    }
}

Potion *createAmbrosia() {
    Potion *potion = new Potion(
        "ambrosia",
        "It is the food of the gods, or so it is said. I'm sure they won't be angry if I take just a little bite (random between +15 health, +25 health, and -20 health).",
        1,
        ambrosiaUseFunction
    );
    return potion;
}

void mysteriousWhitePowderUseFunction(Player *player) {
    int choices[5] = {5, 15, 25, -10, -20};
    std::srand(std::time(nullptr));
    int choice = choices[std::rand() % 5];
    
    if (choice < 0) {  // reduce player stagger
        ActiveEffect *effect = new ActiveEffect("player stagger", choice, 0, -2);
        ActiveEffect *hiddenEffect = new ActiveEffect("player stagger", 50 - choice, 5, -2);
        player->addToActiveEffects(effect);
        player->addToActiveEffects(hiddenEffect);
        std::cout << "I feel... strange (-" << choice << "% stagger next combat duel).\n\n";
    } else {  // increase player health
        player->gainHealth(choice);
        std::cout << "I feel... strange (" << player->getCurrentHealth() << "/" << player->getStartingHealth() << " health).\n\n";
    }
}

Potion *createMysteriousWhitePowder() {
    Potion *potion = new Potion(
        "mysterious white powder",
        "I was told that a couple of pinches will invigorate me from even the deepest of stupors (random between +5 health, +15 health, +25 health, -10% player stagger, or -20% player stagger).",
        1,
        mysteriousWhitePowderUseFunction
    );
    return potion;
}

#include "Shields.hpp"

Shield *createPaperShield() {
    Shield *shield = new Shield(
        "paper shield",
        "They actually make these things? I thought it was just a metaphor.\n",
        1, 1, 0, 0,
        NULL
    );
    shield->appendToDescription(shield->getStatString());
    return shield;
}

Shield *createWoodenShield() {
    Shield *shield = new Shield(
        "wooden shield",
        "Light and mobile, it can be easily discarded when necessary.\n",
        5, 10, 0, 0,
        NULL
    );
    shield->appendToDescription(shield->getStatString());
    return shield;
}

Shield *createMetalShield() {
    Shield *shield = new Shield(
        "metal shield",
        "It is more versatile than a wooden shield, but also more cumbersome. Perhaps the enemy may be dazzled by its shiny surface?\n",
        10, 15, 5, 1,
        NULL
    );
    shield->appendToDescription(shield->getStatString());
    return shield;
}

Shield *createAegisReplica() {
    ActiveEffect *effect = new ActiveEffect("enemy stagger", 50, 0, 0);
    Shield *shield = new Shield(
        "Aegis replica",
        "This cannot be the real Aegis as I am not stone, but this is a most terrifying replica.\n",
        10, 15, 1, 50,
        effect
    );
    shield->appendToDescription(shield->getStatString());
    return shield;
}

#include "Shields.hpp"

Shield *createPaperShield() {
    Shield *shield = new Shield(
        "paper shield",
        "They actually make these things? I thought it was just a metaphor.",
        1, 1, 0, 0
    );
    return shield;
}

Shield *createWoodenShield() {
    Shield *shield = new Shield(
        "wooden shield",
        "Light and mobile, it can be easily discarded when necessary.",
        5, 10, 0, 0
    );
    return shield;
}

Shield *createMetalShield() {
    Shield *shield = new Shield(
        "metal shield",
        "It is more versatile than a wooden shield, but also more cumbersome. Perhaps the enemy may be dazzled by its shiny surface?",
        10, 15, 5, 1
    );
    return shield;
}

Shield *createAegisReplica() {
    Shield *shield = new Shield(
        "Aegis replica",
        "This cannot be the real Aegis as I am not stone, but this is a most terrifying replica.",
        10, 15, 1, 100
    );
    return shield;
}

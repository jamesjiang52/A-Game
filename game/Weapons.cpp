#include "Weapons.hpp"

Weapon *createStoneSword() {
    Weapon *weapon = new Weapon(
        "stone sword", 
        "A slab of rock sharpened to carry a point, it is awkward to handle and difficult to maneuver.",
        3, 3, 10, 0,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createBronzeSword() {
    Weapon *weapon = new Weapon(
        "bronze sword",
        "Approximately sixty centimetres in length, this blade is adorned with a crude wooden hilt.",
        4, 4, 0, 0,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createIronSword() {
    Weapon *weapon = new Weapon(
        "iron sword",
        "Your everyday sword found within castle armouries, it bends rather easily but does not break.",
        5, 8, 5, 0,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createWarRapier() {
    Weapon *weapon = new Weapon(
        "war rapier",
        "A long thin blade with a very sharp tip, it has an intricately complex hilt that wraps around to protect the hand of its user. Due to its slenderness it is much more suited for nimble thrusting attacks rather than sweeping cuts.",
        3, 14, -5, 0,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createWarSabre() {
    Weapon *weapon = new Weapon(
        "war sabre",
        "A curved blade provides a large cutting edge, great for finishing off enemies that refuse to die. The hilt is adorned with gemstones that glow in the darkness.",
        5, 16, 5, 0,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createStarSword() {
    Weapon *weapon = new Weapon(
        "star sword",
        "Made from the remains of a fallen star, the blade is as pale as glass, impossibly sharp, and inflicts a cool burn to those who are cut by it.",
        3, 12, -5, 0,
        0, 0, 2,
        false
    );
    return weapon;
}

Weapon *createFireSword() {
    Weapon *weapon = new Weapon(
        "fire sword",
        "Flame flickers constantly along the blade; I wonder how the fire doesn't burn out?",
        5, 12, 0, 10,
        0, 1, 3,
        false
    );
    return weapon;
}

Weapon *createSpear() {
    Weapon *weapon = new Weapon(
        "spear",
        "A long handle of polished wood carries a sharp tip - an ideal weapon to keep the enemy at a distance.",
        1, 7, -5, 0,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createRustyMace() {
    Weapon *weapon = new Weapon(
        "rusty mace",
        "Once a formidable weapon, its spiked head is slowly rusting away, leaving behind a mess of iron and flakes.",
        8, 6, 15, 0,
        10, 0, 0,
        true
    );
    return weapon;
}

Weapon *createTwoHandedMace() {
    Weapon *weapon = new Weapon(
        "two-handed mace",
        "This fearsome weapon is almost a metre-and-a-half long and carries a spiked steel head capable of smashing through even the sturdiest of armor.",
        7, 13, 20, 0,
        15, 0, 0,
        true
    );
    return weapon;
}

Weapon *createBattleAxe() {
    Weapon *weapon = new Weapon(
        "battle axe",
        "With a crescent-shaped head and a long thick handle, this axe was modified from its woodcutting cousin to become a terror on the battlefield.",
        7, 13, 15, 0,
        10, 0, 0,
        true
    );
    return weapon;
}

Weapon *createMorningstar() {
    Weapon *weapon = new Weapon(
        "morningstar",
        "A cruel spiked ball is connected by chain to the head of a wooden rod.",
        6, 10, 10, 0,
        10, 0, 0,
        true
    );
    return weapon;
}

Weapon *createMorningstarAgainstShield() {
    Weapon *weapon = new Weapon(
        "morningstar",
        "A cruel spiked ball is connected by chain to the head of a wooden rod.",
        6, 10, 10, 25,
        50, 0, 0,
        true
    );
    return weapon;
}

Weapon *createWoodenDagger() {
    Weapon *weapon = new Weapon(
        "wooden dagger",
        "Crudely carved into a point with a handle, I guess it can be used as a distraction.",
        0, 1, 0, 1,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createDiamondDagger() {
    Weapon *weapon = new Weapon(
        "diamond dagger",
        "Extremely hard but perpetually blunt, it is in theory a good weapon, but is more suited for ceremonial purposes.",
        0, 2, 0, 1,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createObsidianDagger() {
    Weapon *weapon = new Weapon(
        "obsidian dagger",
        "Sharp beyond imagination, and with a gold-plated hilt, it is worth fortunes.",
        0, 6, -10, 1,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createTrident() {
    Weapon *weapon = new Weapon(
        "trident",
        "A majestic three-pronged head is mounted on a long handle adorned with life-like carved sea creatures.",
        5, 8, 0, 5,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createWhip() {
    Weapon *weapon = new Weapon(
        "whip",
        "Glowing a sinister red in the torchlight, any hit from this must be painful.",
        2, 6, 0, 10,
        -50, 0, 0,
        false
    );
    return weapon;
}

Weapon *createStone() {
    Weapon *weapon = new Weapon(
        "stone",
        "This is impressively round... and grey... and smooth... who am I kidding at least I won't miss it when I throw it.",
        0, 0, 0, 0,
        0, 0, 0,
        false
    );
    return weapon;
}

Weapon *createExcalibur() {
    Weapon *weapon = new Weapon(
        "Excalibur",
        "Could this be... the legendary sword of King Arthur? Something seems slightly off, but I can't seem to put my finger on it...",
        6, 12, 0, 0,
        0, 0, 0,
        true
    );
    return weapon;
}

Weapon *createExcaliburEnhanced() {
    Weapon *weapon = new Weapon(
        "Excalibur",
        "The legendary sword of King Arthur is complete with the rainbow stone in its pommel!",
        6, 24, 0, 0,
        0, 0, 0,
        true
    );
    return weapon;
}

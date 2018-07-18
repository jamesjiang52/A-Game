#include "Armors.hpp"

Armor *createStreetClothes() {
    Armor *armor = new Armor(
        "street clothes",
        "The uniform of a lowly soldier consists of a navy coat, black pants, and sturdy boots.\n",
        1, 1, 0, 0,
        NULL
    );
    armor->appendToDescription(armor->getStatString());
    return armor;
}

Armor *createLeatherJacket() {
    Armor *armor = new Armor(
        "leather jacket",
        "While impressive looking, it is not able to withstand much.\n",
        1, 2, 0, 1,
        NULL
    );
    armor->appendToDescription(armor->getStatString());
    return armor;
}

Armor *createChainmail() {
    Armor *armor = new Armor(
        "chainmail",
        "Made of interlocking iron rings that are welded shut, it can withstand blows but has several vulnerable areas. It is also exceedingly heavy.\n",
        20, 20, 20, 0,
        NULL
    );
    armor->appendToDescription(armor->getStatString());
    return armor;
}

Armor *createPlateArmor() {
    Armor *armor = new Armor(
        "plate armor",
        "Made of joined plates of thick steel, it convers the body from head to toe and is impenetrable outside well positioned blows.\n",
        40, 40, 20, 0,
        NULL
    );
    armor->appendToDescription(armor->getStatString());
    return armor;
}

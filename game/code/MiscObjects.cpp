#include "MiscObjects.hpp"

void unlitLanternUseFunction(Player *player) {
    std::cout << "Let there be light!\n\n";
    
    InteractableObject *object = player->getObjectFromString("lantern");
    MiscObject *litLantern = createLantern();
    
    player->removeFromInventory(object);
    player->addToInventory(litLantern);
    
    delete object;
}

MiscObject *createLanternUnlit() {
    MiscObject *object = new MiscObject(
        "lantern",
        "The last embers glow faintly within.",
        1,
        unlitLanternUseFunction
    );
    return object;
}

void lanternUseFunction(Player *player) {
    std::cout << "The lantern is already lit.\n\n";
}

MiscObject *createLantern() {
    MiscObject *object = new MiscObject(
        "lantern",
        "The warm flame bathes me in an amber glow.",
        1,
        lanternUseFunction
    );
    return object;
}

void theArtOfWarUseFunction(Player *player) {
    ActiveEffect *effect = new ActiveEffect("player stagger", -5, 0, -1);
    player->addToActiveEffects(effect, false);  // don't allow duplicates
    
    std::cout << "I am hit with stunning epiphany as a river of knowledge flows over me. My combat acumen is improved (-5% player stagger).\n\n";
}

MiscObject *createTheArtOfWar() {
    MiscObject *object = new MiscObject(
        "The Art of War - Sun Tzu",
        "",
        1,
        theArtOfWarUseFunction
    );
    return object;
}

void stoneUseFunction(Player *player) {
    std::cout << "I toss the stone. It does nothing (-1 encumbrance).\n\n";
    InteractableObject *object = player->getObjectFromString("stone");
    player->removeFromInventory(object);
    player->getLocation()->addInteractableObject(object);
}

MiscObject *createStone() {
    MiscObject *object = new MiscObject(
        "stone",
        "This is impressively round... and grey... and smooth...",
        1,
        stoneUseFunction
    );
    return object;
}

void rainbowStoneUseFunction(Player *player) {
    
}

MiscObject *createRainbowStone() {
    MiscObject *object = new MiscObject(
        "rainbow stone",
        "I hope this stone is more useful than the first...",
        1,
        rainbowStoneUseFunction
    );
    return object;
}

void rainbowStoneWithoutExcaliburUseFunction(Player *player) {
    std::cout << "This stone seems too important to toss.\n\n";
}

MiscObject *createRainbowStoneWithoutExcalibur() {
    MiscObject *object = new MiscObject(
        "rainbow stone",
        "I hope this stone is more useful than the first...",
        1,
        rainbowStoneWithoutExcaliburUseFunction
    );
    return object;
}

void quillUseFunction(Player *player) {
    std::cout << "If only I had something to write on...\n\n";
}

MiscObject *createQuill() {
    MiscObject *object = new MiscObject(
        "quill",
        "A feather is sharpened to a fine tip, perfect for penmanship of the highest calibre.",
        1,
        quillUseFunction
    );
    return object;
}

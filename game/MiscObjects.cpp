#include "MiscObjects.hpp"

InteractableObject *createLanternUnlit() {
    InteractableObject *object = new InteractableObject(
        "lantern",
        "The last embers glow faintly within.\n",
        1
    );
    object->setUseMessage("Let there be light!");
    return object;
}

InteractableObject *createLantern() {
    InteractableObject *object = new InteractableObject(
        "lantern",
        "The warm flame bathes me in an amber glow.\n",
        1
    );
    object->setUseMessage("The lantern is already lit.");
    return object;
}

InteractableObject *createTheArtOfWar() {
    InteractableObject *object = new InteractableObject(
        "The Art of War - Sun Tzu",
        "",
        1
    );
    object->setUseMessage("I am hit with stunning epiphany as a river of knowledge flows over me. My combat acumen is improved (-5% player stagger).\n");
    return object;
}

InteractableObject *createStone() {
    InteractableObject *object = new InteractableObject(
        "stone",
        "This is impressively round... and grey... and smooth...\n",
        1
    );
    object->setUseMessage("");
    return object;
}

InteractableObject *createRainbowStone() {
    InteractableObject *object = new InteractableObject(
        "rainbow stone",
        "I hope this stone is more useful than the first...\n",
        1
    );
    object->setUseMessage("");
    return object;
}

InteractableObject *createRainbowStoneWithoutExcalibur() {
    InteractableObject *object = new InteractableObject(
        "rainbow stone",
        "I hope this stone is more useful than the first...\n",
        1
    );
    object->setUseMessage("");
    return object;
}

InteractableObject *createQuill() {
    InteractableObject *object = new InteractableObject(
        "quill",
        "A feather is sharpened to a fine tip, perfect for penmanship of the highest calibre.\n",
        1
    );
    object->setUseMessage("If only I had something to write on...");
    return object;
}

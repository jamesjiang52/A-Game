#include "Journal.hpp"

const int MILLISECONDS_DELAY_BETWEEN_CHARACTERS = 40;
const int MILLISECONDS_DELAY_ON_PAUSE_SHORT = 200;
const int MILLISECONDS_DELAY_ON_PAUSE_LONG = 500;
const int LONG_PAUSE = 1000;

void printJournalHeader() {
    std::cout << "== Begin journal entry ==\n\n";
}

void printJournalFooter() {
    std::cout << "\n\n== End journal entry ==\n\n";
}

void printStringWriting(std::string str, bool allowLongPauses) {
    if (allowLongPauses) {
        for (char& c : str) {
            std::cout << c;
            if ((c == ',') || (c == ';') || (c == ':')) {
                Sleep(MILLISECONDS_DELAY_ON_PAUSE_SHORT);
            } else if ((c == '.') || (c == '?') || (c == '!')) {
                Sleep(MILLISECONDS_DELAY_ON_PAUSE_LONG);
            } else {
                Sleep(MILLISECONDS_DELAY_BETWEEN_CHARACTERS);
            }
        }
    } else {
        for (char& c : str) {
            std::cout << c;
            Sleep(MILLISECONDS_DELAY_BETWEEN_CHARACTERS);
        }
    }
}

void printTitlePage(std::string name) {
    std::cout << "== Begin title page ==\n\n";
    std::cout << name << "'s Journal";
    std::cout << "\n\n== End title page ==\n\n";
}

void printOutsideFortressJournalEntry(bool slow) {
    printJournalHeader();
    
    if (slow) {
        Sleep(LONG_PAUSE);
        printStringWriting("7:34pm", false);
        Sleep(LONG_PAUSE);
        std::cout << "\n\n";
        printStringWriting("Ten years. Ten years of mindless back-breaking labour, ten years of constant under-appreciation, ten "
                     "years of verbal abuse, ten years of manipulation and deceit. Ten years endured, and for what? "
                     "Demotion and an official reprimand for \"unprofessional\" conduct. These were the thoughts that raged "
                     "through my mind as I approached Reinbreaker, the towering presence that loomed over the entire "
                     "valley, its impregnability famed across the land. Considered the sole reason why the west had "
                     "remained relatively unmarred after the many years of war, the fortress stood proudly along the "
                     "river, shining as a beacon of imperial might. Tonight, this will change, its illusion of power "
                     "destroyed, the mighty empire brought to its knees, and I shall be the cause. My name once revered as "
                     "the next imperator will now be spit upon alongside the dregs and scum, but vengeance must have its "
                     "due.");
        Sleep(LONG_PAUSE);
    } else {
        std::cout << "7:34pm\n\n";
        std::cout << "Ten years. Ten years of mindless back-breaking labour, ten years of constant under-appreciation, ten "
                     "years of verbal abuse, ten years of manipulation and deceit. Ten years endured, and for what? "
                     "Demotion and an official reprimand for \"unprofessional\" conduct. These were the thoughts that raged "
                     "through my mind as I approached Reinbreaker, the towering presence that loomed over the entire "
                     "valley, its impregnability famed across the land. Considered the sole reason why the west had "
                     "remained relatively unmarred after the many years of war, the fortress stood proudly along the "
                     "river, shining as a beacon of imperial might. Tonight, this will change, its illusion of power "
                     "destroyed, the mighty empire brought to its knees, and I shall be the cause. My name once revered as "
                     "the next imperator will now be spit upon alongside the dregs and scum, but vengeance must have its "
                     "due.";
    }
    
    printJournalFooter();
}

void journal(int questStage, std::string name) {  // called if player input is "journal"
    std::cout << "\nI open my journal. Use \"next\" and \"previous\" to navigate pages, or \"journal\" to close.\n\n";
    printTitlePage(name);
    
    void (*journalEntries[])(bool slow) = {printOutsideFortressJournalEntry};
    int currentIndex = -1;  // an index of -1 specifies the title page
    
    std::string playerInput;
    do {
        std::getline(std::cin, playerInput);
        std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
        playerInput = stripSpaces(playerInput);
        
        if (playerInput == "next") {
            std::cout << "\n";
            if (currentIndex < questStage) {
                currentIndex += 1;
                journalEntries[currentIndex](false);
            } else {
                journalEntries[currentIndex](false);
            }
        } else if (playerInput == "previous") {
            std::cout << "\n";
            if (currentIndex < 1) {
                printTitlePage(name);
            } else {
                currentIndex -= 1;
                journalEntries[currentIndex](false);
            }
        } else if (playerInput != "journal") {
            std::cout << "Use \"next\" and \"previous\" to navigate pages, or \"journal\" to close.\n\n";
        }
    } while (playerInput != "journal");
    
    std::cout << "\n";
}

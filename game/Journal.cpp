#include "Journal.hpp"

const int MILLISECONDS_DELAY_BETWEEN_CHARACTERS = 20;
const int LONG_MILLISECONDS_DELAY = 1000;

void printJournalHeader() {
    std::cout << "== Begin journal entry ==\n\n";
}

void printJournalFooter() {
    std::cout << "\n\n== End journal entry ==\n\n";
}

void printTitlePage(std::string name) {
    std::cout << std::toupper(name[0]) << name.substr(1, name.length() - 1) << "'s Journal\n\n"
}

void printStringWriting(std::string str) {
    for (char& c : str) {
        std::cout << c;
        std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS_DELAY_BETWEEN_CHARACTERS));
    }
}

void printOutsideFortressJournalEntry(bool slow) {
    printJournalHeader();
    
    if (slow) {
        std::this_thread::sleep_for(std::chrono::milliseconds(LONG_MILLISECONDS_DELAY));
        printStringWriting("7:34pm");
        std::this_thread::sleep_for(std::chrono::milliseconds(LONG_MILLISECONDS_DELAY));
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
        std::this_thread::sleep_for(std::chrono::milliseconds(LONG_MILLISECONDS_DELAY));
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
    std::cout << "I open my journal. Use \"next\" and \"previous\" to navigate pages, or \"journal\" to close.\n\n";
    printTitlePage(name);
    do {
        std::string playerInput;
        std::getline(std::cin, playerInput);
        std::transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);  // convert input to all lowercase
        playerInput = stripSpaces(playerInput);
    } while (playerInput != "journal");
}

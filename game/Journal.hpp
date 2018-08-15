#ifndef JOURNAL_H
#define JOURNAL_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "Utilities.hpp"

void printJournalHeader();
void printJournalFooter();
void printStringWriting(std::string str, bool allowLongPauses=true);
void printTitlePage(std::string name);
void printOutsideFortressJournalEntry(bool slow=true);
void journal(int questStage, std::string name);  // called if player input is "journal"

#endif

#ifndef JOURNAL_H
#define JOURNAL_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

void printJournalHeader();
void printJournalFooter();
void printTitlePage(std::string name);
void printStringWriting(std::string);
void printOutsideFortressJournalEntry(bool slow=true);
void journal(int questStage, std::string name);  // called if player input is "journal"

#endif

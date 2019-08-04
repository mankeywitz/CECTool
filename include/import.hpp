#pragma once

#include "streetpass/StreetpassManager.hpp"
#include "common/STDirectory.hpp"

void displayImportMenu(Streetpass::StreetpassManager& sm);
void displayImportSlotSelection(Streetpass::StreetpassManager& sm, u8 slotNum);
void displayImportStreetpassBoxSelection(Streetpass::StreetpassManager& sm, u8 slotNum, u32 numOfStreetpasses);
void displayImportStreetpassSelection(Streetpass::StreetpassManager& sm, u8 SlotNum, u32 inboxCount, u32 inboxLimit, const std::string& fileName);

void importMenu(Streetpass::StreetpassManager& sm);
void importStreetpassMenu(Streetpass::StreetpassManager& sm, u8 slotNum, STDirectory& importDirectory, const std::string& importPath);

void importBoxes(bool del);
void importBox(Streetpass::StreetpassManager& sm, u8 slotNum);
void importMessages(Streetpass::StreetpassManager& sm, u8 slotNum);

void importStreetpasses(Streetpass::StreetpassManager& sm, Streetpass::MBox& mbox, const std::string& importPath, const std::string& fileName);
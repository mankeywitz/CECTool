#pragma once

#include "streetpass/StreetpassManager.hpp"
#include "common/STDirectory.hpp"

void displayImportMenu(Streetpass::StreetpassManager& sm);
void displayImportSlotSelection(Streetpass::StreetpassManager& sm, const u8 slotNum);
void displayImportStreetpassBoxSelection(Streetpass::StreetpassManager& sm, const u8 slotNum, const u32 numOfStreetpasses);
void displayImportStreetpassSelection(Streetpass::StreetpassManager& sm, const u8 slotNum, const u32 inboxCount, const u32 inboxLimit, const std::string& fileName);

void importMenu(Streetpass::StreetpassManager& sm);
void importStreetpassMenu(Streetpass::StreetpassManager& sm, const u8 slotNum, STDirectory& importDirectory, const std::string& importPath);

void importBox(Streetpass::StreetpassManager& sm, const u8 slotNum);
void importStreetpasses(Streetpass::StreetpassManager& sm, Streetpass::MBox& mbox, const std::string& importPath, const std::string& fileName);
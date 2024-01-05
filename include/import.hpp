#pragma once

#include "common/STDirectory.hpp"
#include "common/util.hpp"

#include "streetpass/StreetpassManager.hpp"

void displayImportMenu(Screens& screens, Streetpass::StreetpassManager& sm);
void displayImportSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);
void displayImportStreetpassBoxSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum, const u32 numOfStreetpasses);
void displayImportStreetpassSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum, const u32 inboxCount, const u32 inboxLimit, const std::string& fileName);

void importMenu(Screens& screens, Streetpass::StreetpassManager& sm, const std::string serverRootUrl, const u64 consoleHash);
void importStreetpassMenu(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum, STDirectory& importDirectory, const std::string& importPath);

void importBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);
void importStreetpasses(Screens& screens, Streetpass::StreetpassManager& sm, Streetpass::MBox& mbox, const std::vector<u8>& messageData);

void downloadStreetpass(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum, const std::string serverRootUrl, const u64 consoleHash);
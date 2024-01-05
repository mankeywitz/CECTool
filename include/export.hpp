#pragma once

#include "common/util.hpp"
#include "streetpass/StreetpassManager.hpp"

void displayExportMenu(Screens& screens, Streetpass::StreetpassManager& sm);
void displayExportSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);
void exportMenu(Screens& screens, Streetpass::StreetpassManager& sm);

void exportAllBoxes(Screens& screens, Streetpass::StreetpassManager& sm);
void exportBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);

void exportStreetpassMessage(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);
void exportAllStreetpasses(Screens& screens, Streetpass::StreetpassManager& sm);

void uploadAllStreetpasses(Screens& screens, Streetpass::StreetpassManager& sm, const std::string serverRootUrl, const u64 consoleHash);

#pragma once

#include "common/util.hpp"
#include "streetpass/StreetpassManager.hpp"

void displayDeleteMenu(Screens& screens, Streetpass::StreetpassManager& sm);
void displayDeleteSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);
void deleteMenu(Screens& screens, Streetpass::StreetpassManager& sm);

void deleteBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);
void deleteAllBoxes(Screens& screens, Streetpass::StreetpassManager& sm);

void deleteStreetpassMessage(Screens& screens, Streetpass::StreetpassManager& sm);
void deleteAllStreetpassMessages(Screens& screens, Streetpass::StreetpassManager& sm);
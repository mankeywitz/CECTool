#pragma once

#include "streetpass/StreetpassManager.hpp"

void displayDeleteMenu(Streetpass::StreetpassManager& sm);
void displayDeleteSlotSelection(Streetpass::StreetpassManager& sm, const u8 slotNum);
void deleteMenu(Streetpass::StreetpassManager& sm);

void deleteBox(Streetpass::StreetpassManager& sm, const u8 slotNum);
void deleteAllBoxes(Streetpass::StreetpassManager& sm);

void deleteStreetpassMessage(Streetpass::StreetpassManager& sm);
void deleteAllStreetpassMessages(Streetpass::StreetpassManager& sm);
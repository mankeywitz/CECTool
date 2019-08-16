#pragma once

#include "common/util.hpp"
#include "streetpass/StreetpassManager.hpp"

void displayOpenMenu(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);
void openMenu(Screens& screens, Streetpass::StreetpassManager& sm);

void openBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum);

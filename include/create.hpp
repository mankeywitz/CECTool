#pragma once

#include <3ds/types.h>
#include <memory>
#include <string>

#include "common/STDirectory.hpp"
#include "common/util.hpp"
#include "streetpass/Box.hpp"
#include "streetpass/MBoxList.hpp"
#include "streetpass/StreetpassManager.hpp"

void displayCreateMenu(Screens& screens, Streetpass::StreetpassManager& sm);
void createMenu(Screens& screens, Streetpass::StreetpassManager& sm);

void createBoxFromBackup(Screens& screens, Streetpass::StreetpassManager& sm, STDirectory& backupDirectory, const u8 slotNum);

void displayBackupSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, STDirectory& backupDirectory, const u8 slotNum);
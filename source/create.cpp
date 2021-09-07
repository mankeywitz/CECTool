#include <3ds.h>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <memory>

#include "common/io.hpp"
#include "common/STDirectory.hpp"
#include "common/util.hpp"
#include "create.hpp"
#include "streetpass/BoxInfo.hpp"
#include "streetpass/CecInbox.hpp"
#include "streetpass/CecOutbox.hpp"
#include "streetpass/MBox.hpp"
#include "streetpass/MBoxIcon.hpp"
#include "streetpass/MBoxProgramId.hpp"
#include "streetpass/MBoxTitle.hpp"
#include "streetpass/Message.hpp"
#include "streetpass/OBIndex.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

void displayCreateMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nCreate Menu\n\n");
    printf("[A] Create Box from Exported Backup\n\n");
    //printf("[B] Create New Box\n"); // Need to figure out common defaults
    printf("Press START for Main Menu\n\n");
}

void displayBackupSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, STDirectory& backupDirectory, const u8 slotNum) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nCreate Menu\n\n");
    printf("[A] Select a Box\n\n");
    printf("Press START for Main Menu\n\n");
    for(u8 slotNum = 0; slotNum < backupDirectory.count(); slotNum++) {
        printf("[%x] %s  ", slotNum, backupDirectory.item(slotNum).c_str());
        if (slotNum % 3 == 2) {
            printf("\n");
        }
    }
    consoleSelect(&screens.bottom);
    printf("\n\nSlot Number: [%x]\n\n", slotNum);
}

void createMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    consoleSelect(&screens.bottom);
    u8 slotNum = 0;
    displayCreateMenu(screens, sm);
    
    while (aptMainLoop()) {
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();
        u32 down = hidKeysDown();

        if(down & KEY_A) {
            if(sm.BoxList().NumberOfSlotsUsed() >= sm.BoxList().MaxNumberOfSlots()) {
                printf("Box list is full. Please delete a box and try again.\n");
                waitForInput();
                break;
            }
            std::unique_ptr<STDirectory> backupDirectory =
                std::make_unique<STDirectory>("/3ds/CECTool/export/");
            auto streetpassesIndex = std::find(backupDirectory->mList.begin(), backupDirectory->mList.end(), "streetpasses");
            if(streetpassesIndex != backupDirectory->mList.end()) {
                backupDirectory->mList.erase(streetpassesIndex);
            }

            displayBackupSlotSelection(screens, sm, *backupDirectory, slotNum);
            while (aptMainLoop()) {
                gspWaitForVBlank();
                gfxSwapBuffers();
                hidScanInput();
                down = hidKeysDown();

                if (down & KEY_A) {
                    createBoxFromBackup(screens, sm, *backupDirectory, slotNum);
                    waitForInput();
                    break;
                } else if (down & KEY_DOWN) {
                    if (slotNum > 0) {
                        slotNum--;
                        displayBackupSlotSelection(screens, sm, *backupDirectory, slotNum);
                    }
                } else if (down & KEY_UP) {
                    if (slotNum < backupDirectory->count() - 1) {
                        slotNum++;
                        displayBackupSlotSelection(screens, sm, *backupDirectory, slotNum);
                    }
                } else if (down & KEY_START) {
                    break;
                }
            }
            break;
        } else if(down & KEY_START) {
            break;
        }
    }
}

void createBoxFromBackup(Screens& screens, Streetpass::StreetpassManager& sm, STDirectory& backupDirectory, const u8 slotNum) {
    consoleSelect(&screens.bottom);
    sm.ImportBox(std::stoul(backupDirectory.item(slotNum), nullptr, 16));
}

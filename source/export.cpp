#include <3ds.h>
#include <fstream>
#include <sys/stat.h>

#include "common/base64.hpp"
#include "common/util.hpp"
#include "export.hpp"
#include "streetpass/MBox.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

void displayExportMenu(Streetpass::StreetpassManager& sm) {
    consoleClear();
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nExport Menu\n\n");
    printf("[A] Export a Box\n");
    printf("[B] Export all Boxes\n");
    printf("[X] Export a Streetpass Message\n");
    printf("[Y] Export all Streetpass Messsages\n\n");
    printf("Press START for Main Menu\n\n");
}

void displayExportSlotSelection(Streetpass::StreetpassManager& sm, const u8 slotNum) {
    consoleClear();
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nExport Menu\n\n");
    printf("[A] Select a Box to Export\n\n");
    printf("Press START for Main Menu\n\n");
    printf("Slot Number: [%x]\n\n", slotNum);
}

void exportMenu(Streetpass::StreetpassManager& sm) {
    u8 slotNum = 0;
    displayExportMenu(sm);
    u32 down = hidKeysDown();
    while (aptMainLoop() && !(down & KEY_START)) {
        down = hidKeysDown();
        hidScanInput();

        if (down & KEY_A) {
            displayExportSlotSelection(sm, slotNum);
            while (aptMainLoop() && !(down & KEY_START)) {
                down = hidKeysDown();
                hidScanInput();
                if (down & KEY_A) {
                    exportBox(sm, slotNum);
                    waitForInput();
                    break;
                } else if (down & KEY_DOWN) {
                    if (slotNum > 0) {
                        slotNum--;
                        displayExportSlotSelection(sm, slotNum);
                    }
                } else if (down & KEY_UP) {
                    if (slotNum < sm.BoxList().MaxNumberOfSlots() - 1) {
                        slotNum++;
                        displayExportSlotSelection(sm, slotNum);
                    }
                }
            }
            break;
        } else if (down & KEY_B) {
            exportAllBoxes(sm);
            waitForInput();
            break;
        } else if (down & KEY_X) {
            exportStreetpassMessage(sm, 0);
            waitForInput();
            break;
        } else if (down & KEY_Y) {
            exportAllStreetpasses(sm);
            waitForInput();
            break;
        }
    }
}

void exportAllBoxes(Streetpass::StreetpassManager& sm) {
    for (u8 slotNum = 0; slotNum < sm.BoxList().MaxNumberOfSlots(); slotNum++) {
        exportBox(sm, slotNum);
    }
    printf("All boxes exported.\n");
}

void exportBox(Streetpass::StreetpassManager& sm, const u8 slotNum) {
    sm.BackupBox(slotNum);
}

void exportStreetpassMessage(Streetpass::StreetpassManager& sm, const u8 slotNum) {
    printf("Unimplemented.\n");
}

void exportAllStreetpasses(Streetpass::StreetpassManager& sm) {
    for (u8 slotNum = 0; slotNum < sm.BoxList().MaxNumberOfSlots(); slotNum++) {
        sm.ExportStreetpasses(slotNum);
    }
    printf("All streetpasses exported.\n");
}
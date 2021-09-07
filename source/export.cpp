#include <3ds.h>
#include <fstream>
#include <sys/stat.h>

#include "common/base64.hpp"
#include "export.hpp"
#include "streetpass/MBox.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

void displayExportMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nExport Menu\n\n");
    printf("[A] Export a Box\n");
    printf("[B] Export all Boxes\n");
    printf("[X] Export a Streetpass Message\n");
    printf("[Y] Export all Streetpass Messsages\n\n");
    printf("Press START for Main Menu\n\n");
}

void displayExportSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nExport Menu\n\n");
    printf("[A] Select a Box to Export\n\n");
    printf("Press START for Main Menu\n\n");
    consoleSelect(&screens.bottom);
    printf("Slot Number: [%x]\n\n", slotNum);
}

void exportMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    consoleSelect(&screens.bottom);
    u8 slotNum = 0;
    displayExportMenu(screens, sm);
    while (aptMainLoop()) {
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();
        u32 down = hidKeysDown();

        if (down & KEY_A) {
            displayExportSlotSelection(screens, sm, slotNum);
            while (aptMainLoop()) {
                gspWaitForVBlank();
                gfxSwapBuffers();
                hidScanInput();
                down = hidKeysDown();
                if (down & KEY_A) {
                    exportBox(screens, sm, slotNum);
                    waitForInput();
                    break;
                } else if (down & KEY_DOWN) {
                    if (slotNum > 0) {
                        slotNum--;
                        displayExportSlotSelection(screens, sm, slotNum);
                    }
                } else if (down & KEY_UP) {
                    if (slotNum < sm.BoxList().MaxNumberOfSlots() - 1) {
                        slotNum++;
                        displayExportSlotSelection(screens, sm, slotNum);
                    }
                } else if (down & KEY_START) {
                    break;
                }
            }
            break;
        } else if (down & KEY_B) {
            exportAllBoxes(screens, sm);
            waitForInput();
            break;
        } else if (down & KEY_X) {
            exportStreetpassMessage(screens, sm, 0);
            waitForInput();
            break;
        } else if (down & KEY_Y) {
            exportAllStreetpasses(screens, sm);
            waitForInput();
            break;
        } else if (down & KEY_START) {
            break;
        }
    }
}

void exportAllBoxes(Screens& screens, Streetpass::StreetpassManager& sm) {
    consoleSelect(&screens.bottom);
    for (u8 slotNum = 0; slotNum < sm.BoxList().MaxNumberOfSlots(); slotNum++) {
        exportBox(screens, sm, slotNum);
    }
    printf("All boxes exported.\n");
}

void exportBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    consoleSelect(&screens.bottom);
    sm.BackupBox(slotNum);
    printf("Box exported.\n");
}

void exportStreetpassMessage(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    consoleSelect(&screens.bottom);
    printf("Unimplemented.\n");
}

void exportAllStreetpasses(Screens& screens, Streetpass::StreetpassManager& sm) {
    consoleSelect(&screens.bottom);
    printf("Exporting all streetpasses...\n");
    for (u8 slotNum = 0; slotNum < sm.BoxList().MaxNumberOfSlots(); slotNum++) {
        sm.ExportStreetpasses(slotNum);
    }
    printf("All streetpasses exported.\n");
}
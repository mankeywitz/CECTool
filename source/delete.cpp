#include <3ds.h>

#include "delete.hpp"

void displayDeleteMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nDelete Menu\n\n");
    printf("[A] Delete a Box\n");
    printf("[B] Delete all boxes\n");
    printf("[X] Delete a Streetpass Message\n");
    printf("[Y] Delete all Streetpass Messages in a Box\n\n");
    printf("Press START for Main Menu\n\n");
}

void displayDeleteSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\n");
    printf("Delete Menu\n\n");
    printf("[A] Select a Box to Delete\n\n");
    printf("Press START for Main Menu\n\n");
    consoleSelect(&screens.bottom);
    printf("Slot Number: [%x]\n\n", slotNum);
}

void deleteMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    u8 slotNum = 0;
    displayDeleteMenu(screens, sm);
    u32 down = hidKeysDown();
    while (aptMainLoop() && !(down & KEY_START)) {
        down = hidKeysDown();
        hidScanInput();

        if (down & KEY_A) {
            displayDeleteSlotSelection(screens, sm, slotNum);
            while (aptMainLoop() && !(down & KEY_START)) {
                down = hidKeysDown();
                hidScanInput();
                if (down & KEY_A) {
                    deleteBox(screens, sm, slotNum);
                    waitForInput();
                    break;
                } else if (down & KEY_DOWN) {
                    if (slotNum > 0) {
                        slotNum--;
                        displayDeleteSlotSelection(screens, sm, slotNum);
                    }
                } else if (down & KEY_UP) {
                    if (slotNum < sm.BoxList().MaxNumberOfSlots() - 1) {
                        slotNum++;
                        displayDeleteSlotSelection(screens, sm, slotNum);
                    }
                }
            }
            break;
        } else if (down & KEY_B) {
            deleteAllBoxes(screens, sm);
            waitForInput();
            break;
        } else if (down & KEY_X) {
            deleteStreetpassMessage(screens, sm);
            waitForInput();
            break;
        } else if (down & KEY_Y) {
            deleteAllStreetpassMessages(screens, sm);
            waitForInput();
            break;
        }
    }
}

void deleteBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    consoleSelect(&screens.bottom);
    printf("This will delete the box in slot [%x].\n", slotNum);
    printf("Are you sure?\n");
    printf("A: Yes\t B: No\n");
    hidScanInput();
    while (aptMainLoop() && !(hidKeysDown() & KEY_A) && !(hidKeysDown() & KEY_B)) {
        hidScanInput();
        if (hidKeysDown() & KEY_A) {
            sm.DeleteBox(slotNum);
            printf("Box deleted.\n");
            waitForInput();
            break;
        }
        if (hidKeysDown() & KEY_B) {
            printf("Canceled.\n");
            waitForInput();
            break;
        }
    }
}

void deleteAllBoxes(Screens& screens, Streetpass::StreetpassManager& sm) {
    consoleSelect(&screens.bottom);
    printf("This will delete all boxes.\n");
    printf("Are you sure?\n");
    printf("A: Yes\t B: No\n");
    hidScanInput();
    while (aptMainLoop() && !(hidKeysDown() & KEY_A) && !(hidKeysDown() & KEY_B)) {
        hidScanInput();
        if (hidKeysDown() & KEY_A) {
            sm.DeleteAllBoxes();
            printf("All boxes deleted.\n");
            waitForInput();
            break;
        }
        if (hidKeysDown() & KEY_B) {
            printf("Canceled.\n");
            waitForInput();
            break;
        }
    }
}

void deleteStreetpassMessage(Screens& screens, Streetpass::StreetpassManager& sm) {
    consoleSelect(&screens.bottom);
    printf("Unimplemented.\n");
}

void deleteAllStreetpassMessages(Screens& screens, Streetpass::StreetpassManager& sm) {
    consoleSelect(&screens.bottom);
    printf("Unimplemented.\n");
}

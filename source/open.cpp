#include <3ds.h>
#include <memory>
#include <string>

#include "common/util.hpp"
#include "open.hpp"
#include "streetpass/MBox.hpp"

void displayOpenMenu(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nOpen Menu\n\n");
    printf("[A] Select a Box to Open\n\n");
    printf("Press START for Main Menu\n\n");
    consoleSelect(&screens.bottom);
    printf("Slot Number: [%x]\n\n", slotNum);
}

void openMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    u8 slotNum = 0;
    displayOpenMenu(screens, sm, slotNum);
    while (aptMainLoop()) {
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();
        u32 down = hidKeysDown();

        if (down & KEY_A) {
            openBox(screens, sm, slotNum);
            waitForInput();
            break;
        } else if (down & KEY_DOWN) {
            if (slotNum > 0) {
                slotNum--;
                displayOpenMenu(screens, sm, slotNum);
            }
        } else if (down & KEY_UP) {
            if (slotNum < sm.BoxList().MaxNumberOfSlots() - 1) {
                slotNum++;
                displayOpenMenu(screens, sm, slotNum);
            }
        } else if (down & KEY_START) {
            break;
        }
    }
}

void openBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    const std::string boxName = sm.BoxList().BoxNames()[slotNum];
    std::shared_ptr<Streetpass::MBox> mbox = sm.OpenBox(slotNum);

    const std::string rootCecPath = "/CEC/";
    const std::string mboxPath = rootCecPath + boxName + "/";
    //const std::string mboxInboxPath = mboxPath + "InBox___/";
    //const std::string mboxOutboxPath = mboxPath + "OutBox__/";

    if (mbox) {
        ClearScreen(&screens.bottom);
        printf("[%x] Box Id: %s\n\n", slotNum, boxName.c_str());
        printf("%s\n", mboxPath.c_str());
        printf("  InBox___/\n");
        printf("  OutBox__/\n");
        printf("  MBoxData.001\n");
        printf("  MBoxData.010\n");
        printf("  MBoxData.050\n");
        printf("  MBoxInfo____\n\n\n");

        printf("[Hex Dump of MBoxInfo____]\n");
        sm.HexDump(mbox->data());

        printf("[Hex Dump of MBoxData.010]\n");
        sm.HexDump(mbox->BoxTitle().data());

        printf("[Hex Dump of MBoxData.050]\n");
        sm.HexDump(mbox->BoxProgramId().data());
        consoleSelect(&screens.top);
    }
}
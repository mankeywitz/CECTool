#include <3ds.h>
#include <fstream>

#include "import.hpp"
#include "common/io.hpp"
#include "common/util.hpp"

#include "streetpass/MBox.hpp"
#include "http.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

void displayImportMenu(Screens& screens, Streetpass::StreetpassManager& sm) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nDownload Menu\n\n");
    printf("[A] Download Streetpass Messages\n\n");
    printf("Press START for Main Menu\n\n");
}

void displayImportSlotSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nImport Menu\n\n");
    printf("[A] Select a Box to Import\n\n");
    printf("Press START for Main Menu\n\n");
    consoleSelect(&screens.bottom);
    printf("Slot Number: [%x]\n\n", slotNum);
}

void displayImportStreetpassBoxSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes(slotNum);
    printf("\n\nImport Menu\n\n");
    printf("[A] Select a box to download streetpasses to\n\n");
    printf("Press START for Main Menu\n\n");
    printf("Slot Number: [%x]\n", slotNum);
}

void displayImportStreetpassSelection(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum, const u32 inboxCount, const u32 inboxLimit, const std::string& fileName) {
    ClearScreens(screens);
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nImport Menu\n\n");
    printf("[A] Select a Streetpass to Import\n\n");
    printf("Press START for Main Menu\n\n");
    consoleSelect(&screens.bottom);
    printf("Slot Number: [%x],  Inbox Limit: [%lu/%lu]\n\n", slotNum, inboxCount, inboxLimit);
    printf("%s\n\n", fileName.c_str());
}

void importMenu(Screens& screens, Streetpass::StreetpassManager& sm, const std::string serverRootUrl, const u64 consoleHash) {
    consoleSelect(&screens.bottom);
    u8 slotNum = 0;
    displayImportMenu(screens, sm);
    while (aptMainLoop()) {
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();
        u32 down = hidKeysDown();

        std::vector<std::string> currentBoxNames = sm.BoxList().BoxNames();
        std::string importPath = "/3ds/CECTool/import/streetpasses/" + currentBoxNames[slotNum] + "/";
        std::unique_ptr<STDirectory> importDirectory =
            std::make_unique<STDirectory>(importPath);

        displayImportStreetpassBoxSelection(screens, sm, slotNum);
        while (aptMainLoop()) {
            gspWaitForVBlank();
            gfxSwapBuffers();
            hidScanInput();
            down = hidKeysDown();

            if (down & KEY_A) {
                downloadStreetpass(screens, sm, slotNum, serverRootUrl, consoleHash);
                break;
            } else if (down & KEY_DOWN) {
                if (slotNum > 0) {
                    slotNum--;
                    displayImportStreetpassBoxSelection(screens, sm, slotNum);
                }
            } else if (down & KEY_UP) {
                if (slotNum < sm.BoxList().MaxNumberOfSlots() - 1) {
                    slotNum++;
                    displayImportStreetpassBoxSelection(screens, sm, slotNum);
                }
            } else if (down & KEY_START) {
                break;
            }
        }
        break;
    }
}

void downloadStreetpass(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum, const std::string serverRootUrl, const u64 consoleHash) {
    std::shared_ptr<Streetpass::MBox> mbox = sm.OpenBox(slotNum);
    std::vector<std::string> currentBoxNames = sm.BoxList().BoxNames();
    Result ret = 0;

    consoleSelect(&screens.bottom);

    if(mbox) {
        const std::string boxName = currentBoxNames[slotNum];
        printf("Downloading message for %s\n", boxName.c_str());

        std::vector<u8> messageData;

        ret = downloadMessage(serverRootUrl, boxName, consoleHash, messageData);
        if(R_FAILED(ret)) {
            printf("Download failed\n");
            return;
        }

        printf("Importing downloaded message\n");
        importStreetpasses(screens, sm, *mbox, messageData);
        printf("Importing complete\n");
    }
}

void importBox(Screens& screens, Streetpass::StreetpassManager& sm, const u8 slotNum) {
    consoleSelect(&screens.bottom);
    printf("Unimplemented.\n");
}

void importStreetpasses(Screens& screens, Streetpass::StreetpassManager& sm, Streetpass::MBox& mbox, const std::vector<u8>& messageData) {
    consoleSelect(&screens.bottom);

    time_t currentTime;
    struct tm timeStruct;
    time(&currentTime);
    gmtime_r(&currentTime, &timeStruct);

    CecTimestamp timeReceived;
    timeReceived.hour = timeStruct.tm_hour;
    timeReceived.minute = timeStruct.tm_min;
    timeReceived.second = timeStruct.tm_sec;
    timeReceived.day = timeStruct.tm_mday;
    timeReceived.month = timeStruct.tm_mon;
    timeReceived.year = timeStruct.tm_year + 1900;

    std::unique_ptr<Streetpass::Message> message = std::make_unique<Streetpass::Message>(messageData);
    message->MessageHeader().received = timeReceived;
    message->MessageHeader().newFlag = 1;
    message->MessageHeader().unopened = 1;
    mbox.Inbox().AddMessage(*message);

    Result res = cecdWriteMessage(mbox.ProgramId(), false, message->MessageId().data, sizeof(CecMessageId), message->data().data(),message->MessageSize());
    if (R_FAILED(res)) {
        printf("Inbox WriteMessage Failed: %lX\n", res);
        //return res;
    }

    res = cecdOpenAndWrite(mbox.Inbox().Info().data().data(), mbox.Inbox().Info().FileSize(), mbox.ProgramId(), CEC_PATH_INBOX_INFO,
                           CEC_WRITE | CEC_CREATE | CEC_CHECK);
    if (R_FAILED(res)) {
        printf("Inbox BoxInfo OpenAndWrite Failed: %lX\n", res);
        //return res;
    }

    mbox.Header().lastReceived = timeReceived;
    res = cecdOpenAndWrite(mbox.data().data(), sizeof(CecMBoxInfoHeader), mbox.ProgramId(), CEC_PATH_MBOX_INFO,
                             CEC_WRITE | CEC_CREATE | CEC_CHECK);
    if (R_FAILED(res)) {
        printf("MBoxInfo OpenAndWrite Failed: %lX\n", res);
        //return res;
    }
}

#include <3ds.h>
#include <fstream>

#include "import.hpp"
#include "common/io.hpp"
#include "common/util.hpp"

#include "streetpass/MBox.hpp"

extern "C" {
#include "3ds/services/cecdu.h"
}

void displayImportMenu(Streetpass::StreetpassManager& sm) {
    consoleClear();
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nImport Menu\n\n");
    printf("[A] Import Box\n");
    printf("[B] Import Messages\n");
    printf("[X] Import Streetpasses\n\n");
    printf("Press START for Main Menu\n\n");
}

void displayImportSlotSelection(Streetpass::StreetpassManager& sm, u8 slotNum) {
    consoleClear();
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nImport Menu\n\n");
    printf("[A] Select a Box to Import\n\n");
    printf("Press START for Main Menu\n\n");
    printf("Slot Number: [%x]\n\n", slotNum);
}

void displayImportStreetpassBoxSelection(Streetpass::StreetpassManager& sm, u8 slotNum, u32 numOfStreetpasses) {
    consoleClear();
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nImport Menu\n\n");
    printf("[A] Select a Box to Import Streetpasses from\n\n");
    printf("Press START for Main Menu\n\n");
    printf("Slot Number: [%x],  Streetpasses Available: [%lu]\n\n", slotNum, numOfStreetpasses);
}

void displayImportStreetpassSelection(Streetpass::StreetpassManager& sm, u8 slotNum, u32 inboxCount, u32 inboxLimit, const std::string& fileName) {
    consoleClear();
    printf("CECTool\n\n");
    sm.ListBoxes();
    printf("\n\nImport Menu\n\n");
    printf("[A] Select a Streetpass to Import\n\n");
    printf("Press START for Main Menu\n\n");
    printf("Slot Number: [%x],  Inbox Limit: [%lu/%lu]\n\n", slotNum, inboxCount, inboxLimit);
    printf("%s\n\n", fileName.c_str());
}

void importMenu(Streetpass::StreetpassManager& sm) {
    u8 slotNum = 0;
    u32 down = hidKeysDown();
    displayImportMenu(sm);
    while (aptMainLoop() && !(down & KEY_START)) {
        down = hidKeysDown();
        hidScanInput();

        if (down & KEY_A) {
            importBox(sm, slotNum);
            waitForInput();
            break;
        } else if (down & KEY_B) {
            importMessages(sm, slotNum);
            waitForInput();
            break;
        } else if (down & KEY_X) {
            std::unique_ptr<STDirectory> streetpassesDirectory =
                std::make_unique<STDirectory>("/3ds/CECTool/import/streetpasses/");

            if(streetpassesDirectory->count() < 1) {
                printf("/3ds/CECTool/import/streetpasses is empty\n");
                printf("Nothing to import. Returning to main menu.\n");
                waitForInput();
                break;
            }
            std::vector<std::string> currentBoxNames = sm.BoxList().BoxNames();
            std::string importPath = "/3ds/CECTool/import/streetpasses/" + currentBoxNames[slotNum] + "/";
            std::unique_ptr<STDirectory> importDirectory =
                std::make_unique<STDirectory>(importPath);
            u32 numOfStreetpasses = importDirectory->count();

            displayImportStreetpassBoxSelection(sm, slotNum, numOfStreetpasses);
            while (aptMainLoop() && !(down & KEY_START)) {
                down = hidKeysDown();
                hidScanInput();
                if (down & KEY_A) {
                    importStreetpassMenu(sm, slotNum, *importDirectory, importPath);
                    waitForInput();
                    break;
                } else if (down & KEY_DOWN) {
                    if (slotNum > 0) {
                        slotNum--;
                        importPath = "/3ds/CECTool/import/streetpasses/" + currentBoxNames[slotNum] + "/";
                        importDirectory = std::make_unique<STDirectory>(importPath);
                        numOfStreetpasses = importDirectory->count();
                        displayImportStreetpassBoxSelection(sm, slotNum, numOfStreetpasses);
                    }
                } else if (down & KEY_UP) {
                    if (slotNum < sm.BoxList().MaxNumberOfSlots() - 1) {
                        slotNum++;
                        importPath = "/3ds/CECTool/import/streetpasses/" + currentBoxNames[slotNum] + "/";
                        importDirectory = std::make_unique<STDirectory>(importPath);
                        numOfStreetpasses = importDirectory->count();
                        displayImportStreetpassBoxSelection(sm, slotNum, numOfStreetpasses);
                    }
                }
            }
            break;
        }
    }
}

void importStreetpassMenu(Streetpass::StreetpassManager& sm, u8 slotNum, STDirectory& importDirectory, const std::string& importPath) {
    std::shared_ptr<Streetpass::MBox> mbox = sm.OpenBox(slotNum);
    if (mbox) {
        u32 inboxCount = mbox->Inbox().Info().NumberOfMessages();
        u32 inboxLimit = mbox->Inbox().Info().MaxMessages();
        u32 streetpassesAvailable = importDirectory.count();
        u32 streetpassIndex = 0;

        u32 down = hidKeysDown();
        displayImportStreetpassSelection(sm, slotNum, inboxCount, inboxLimit, importDirectory.item(streetpassIndex));
        while (aptMainLoop() && !(down & KEY_START)) {
            down = hidKeysDown();
            hidScanInput();
            if (down & KEY_A) {
                if (inboxCount < inboxLimit) {
                    importStreetpasses(sm, *mbox, importPath, importDirectory.item(streetpassIndex));
                }
                if (inboxCount < mbox->Inbox().Info().NumberOfMessages()) {
                    printf("Importing streetpass complete.");
                    inboxCount = mbox->Inbox().Info().NumberOfMessages();
                } else {
                    printf("Importing streetpass failed.");
                }

                waitForInput();
                displayImportStreetpassSelection(sm, slotNum, inboxCount, inboxLimit, importDirectory.item(streetpassIndex));
                //break;
            } else if (down & KEY_DOWN) {
                if (streetpassIndex > 0) {
                    streetpassIndex--;

                    displayImportStreetpassSelection(sm, slotNum, inboxCount, inboxLimit, importDirectory.item(streetpassIndex));
                }
            } else if (down & KEY_UP) {
                if (streetpassIndex < streetpassesAvailable - 1) {
                    streetpassIndex++;

                    displayImportStreetpassSelection(sm, slotNum, inboxCount, inboxLimit, importDirectory.item(streetpassIndex));
                }
            }
        }
    }
}

void importBoxes(bool deleteBox = false) {

}

void importBox(Streetpass::StreetpassManager& sm, u8 slotNum) {
    printf("Not yet...\n");
}

void importMessages(Streetpass::StreetpassManager& sm, u8 slotNum) {
    printf("Not yet...\n");
}

void importStreetpasses(Streetpass::StreetpassManager& sm, Streetpass::MBox& mbox, const std::string& importPath, const std::string& fileName) {
    std::ifstream in(importPath + fileName, std::ios::in | std::ios::binary);
    in.seekg(0, in.end);
    const u32 messageSize = in.tellg();
    in.seekg(0, in.beg);
    std::vector<u8> messageBuffer(messageSize);
    in.read(reinterpret_cast<char*>(messageBuffer.data()), messageSize);
    in.close();

    time_t currentTime = time(NULL);
    struct tm* timeStruct = gmtime((const time_t *)&currentTime);

    CecTimestamp timeReceived;
    timeReceived.hour = timeStruct->tm_hour;
    timeReceived.minute = timeStruct->tm_min;
    timeReceived.second = timeStruct->tm_sec;
    timeReceived.day = timeStruct->tm_mday;
    timeReceived.month = timeStruct->tm_mon;
    timeReceived.year = timeStruct->tm_year + 1900;

    std::unique_ptr<Streetpass::Message> message = std::make_unique<Streetpass::Message>(messageBuffer);
    message->MessageHeader().received = timeReceived;
    mbox.Inbox().AddMessage(*message);

    Result res = CECDU_WriteMessage(mbox.ProgramId(), false, sizeof(CecMessageId), message->MessageSize(),
                                    message->data().data(), message->MessageId().data);
    if (R_FAILED(res)) {
        printf("Inbox WriteMessage Failed: %lX\n", res);
        //return res;
    }

    res = CECDU_OpenAndWrite(mbox.Inbox().Info().FileSize(), mbox.ProgramId(), CEC_PATH_INBOX_INFO,
                             CEC_WRITE | CEC_CREATE, mbox.Inbox().Info().data().data());
    if (R_FAILED(res)) {
        printf("Inbox BoxInfo OpenAndWrite Failed: %lX\n", res);
        //return res;
    }

    mbox.Header().lastReceived = timeReceived;
    res = CECDU_OpenAndWrite(sizeof(Streetpass::CecMBoxInfoHeader), mbox.ProgramId(), CEC_PATH_MBOX_INFO,
                             CEC_WRITE | CEC_CREATE, mbox.data().data());
    if (R_FAILED(res)) {
        printf("MBoxInfo OpenAndWrite Failed: %lX\n", res);
        //return res;
    }
}

/*
void importBoxes(bool del)
{
    //Result res; currently unused
    MBoxListHeader list;
    u32 size = sizeof(MBoxListHeader);
    CECDU_Open(0x0, CEC_PATH_MBOX_LIST, CEC_READ | CEC_CHECK, nullptr);
    CECDU_Read(size, &list, nullptr);
    STDirectory dir("/3ds/CECTool");
    std::vector<std::string> boxes;
    for (size_t i = 0; i < dir.count(); i++)
    {
        if (dir.folder(i))
        {
            if (io::folderExists("/3ds/CECTool/" + dir.item(i) + "/InBox___"))
            {
                if (dir.item(i).find_first_not_of("1234567890abcdef") == std::string::npos)
                {
                    boxes.push_back(dir.item(i));
                }
            }
        }
    }
    std::vector<int> availableBoxes;
    for (u32 i = 0; i < list.numBoxes; i++)
    {
        if (std::find(boxes.begin(), boxes.end(), list.boxIds[i]) != boxes.end())
        {
            availableBoxes.push_back(i);
        }
    }
    if (availableBoxes.size() > 12)
    {
        printf("Too many boxes!\nI'm not implementing box choices\nin this limited beta.\nWait for a UI, please\n");
        return;
    }
    else
    {
        u32 currentId;
        std::string boxId;
        for (size_t i = 0; i < availableBoxes.size(); i++)
        {
            boxId = list.boxIds[availableBoxes[i]];
            currentId = std::stoul(list.boxIds[availableBoxes[i]], nullptr, 16);
            dir = STDirectory("/3ds/CECTool/" + boxId + "/InBox___");
            Box box(currentId, false);
            for (size_t j = 0; j < dir.good() && dir.count() && box.Messages().size() < box.Info().maxMessages(); j++)
            {
                FILE* in = fopen(("/3ds/CECTool/" + boxId + "/InBox___/" + dir.item(j)).c_str(), "r");
                fseek(in, 0, SEEK_END);
                size_t messageSize = ftell(in);
                fseek(in, 0, SEEK_SET);
                std::vector<u8> buffer(messageSize);
                fread(buffer.data(), 1, messageSize, in);
                fclose(in);

                Message message(buffer);
                box.addMessage(message);
                if (del)
                {
                    remove(("/3ds/CECTool/" + boxId + "/InBox___/" + dir.item(j)).c_str());
                }
            }

            dir = STDirectory("/3ds/CECTool/" + boxId + "/OutBox__");
            for (size_t j = 0; dir.good() && j < dir.count() && box.Messages().size() < box.Info().maxMessages(); j++)
            {
                FILE* in = fopen(("/3ds/CECTool/" + boxId + "/OutBox__/" + dir.item(j)).c_str(), "r");
                fseek(in, 0, SEEK_END);
                size_t messageSize = ftell(in);
                fseek(in, 0, SEEK_SET);
                std::vector<u8> buffer(messageSize);
                fread(buffer.data(), 1, messageSize, in);
                fclose(in);

                Message message(buffer);
                message.getInfo().updateTimes();
                box.addMessage(message);
                if (del)
                {
                    remove(("/3ds/CECTool/" + boxId + "/OutBox__/" + dir.item(j)).c_str());
                }
            }

            if (del)
            {
                remove(("/3ds/CECTool/" + boxId).c_str());
            }
            box.saveInfo();
        }
    }
}
*/

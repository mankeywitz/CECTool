#include <3ds.h>
#include <memory>
#include <sys/stat.h>

#include "common/http.hpp"
#include "common/util.hpp"
#include "create.hpp"
#include "delete.hpp"
#include "export.hpp"
#include "import.hpp"
#include "open.hpp"
#include "streetpass/StreetpassManager.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

using Streetpass::StreetpassManager;

int __stacksize__ = 64 * 1024;

const int HTTP_BUFFER_SIZE = 4 * 1024 * 1024;
const std::string SERVER_VERSION = "0.1.0";
const std::string SERVER_ROOT_URL = "http://192.168.50.153:8000/";
const u32 APP_ID_SALT = 0xD00D;

void cecToolDirectoryCheck(void) {
    mkdir("/3ds/CECTool", 777);
    mkdir("/3ds/CECTool/export", 777);
    mkdir("/3ds/CECTool/export/streetpasses", 777);
    mkdir("/3ds/CECTool/import", 777);
    mkdir("/3ds/CECTool/import/streetpasses", 777);
    mkdir("/3ds/CECTool/tests", 777);
}

Result init(Screens& screens) {
    Result ret = 0;

    gfxInitDefault();
    // Initialize both screens
    consoleInit(GFX_BOTTOM, &screens.bottom);
    consoleInit(GFX_TOP, &screens.top);

    hidInit();
    if (R_FAILED(ret)) {
        printf("Hid Init Failed: %lX\n", ret);
        return ret;
    }

    ret = cecduInit();
    if (R_FAILED(ret)) {
        printf("Cecdu Init Failed: %lX\n", ret);
        return ret;
    }

    ret = httpcInit(HTTP_BUFFER_SIZE);
    if (R_FAILED(ret)) {
        printf("HTTP Init Failed: %lX\n", ret);
        return ret;
    }

    ret = cfguInit();
    if (R_FAILED(ret)) {
        printf("CFGU Init Failed: %lX\n", ret);
        return ret;
    }
    return ret;
}

void shutdown(void) {
    httpcExit();
    cecduExit();
    cfguExit();
}

int main(void) {
    Screens screens{};
    Result ret = init(screens);
    if (R_FAILED(ret)) {
        printf("Init Failed: %lX\n", ret);
        return 1;
    }
    cecToolDirectoryCheck();

    u64 hash = 0;
    ret = CFGU_GenHashConsoleUnique(APP_ID_SALT, &hash);


    std::unique_ptr<StreetpassManager> sm = std::make_unique<StreetpassManager>();
    // Main menu loop; Start to exit
    bool showMenu = true;
    while (aptMainLoop()) {
        if (showMenu) {
            ClearScreens(screens);
            printf("CECTool\n\n");
            sm->ListBoxes();
            printf("\n\nMain Menu\n\n");

            printf("[X] Upload\n");
            printf("[Y] Download\n");
            printf("[R] Test Server\n");

            printf("\nPress START to exit\n");
            printf("Console Specific Hash is 0x%llx\n", hash);
            showMenu = false;
        }
        
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();
        u32 down = hidKeysDown();

        if (down & KEY_X) {
            uploadAllStreetpasses(screens, *sm, SERVER_ROOT_URL, hash);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_Y) {
            importMenu(screens, *sm, SERVER_ROOT_URL, hash);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_R) {
            verifyServer(SERVER_ROOT_URL, SERVER_VERSION);
            //ret = downloadMessage(SERVER_ROOT_URL, "00020800", hash);
            //printf("Return code is %li\n", ret);
            waitForInput();

            showMenu = true;
        } else if (down & KEY_START) {
            break;
        }
    }
    printf("Exiting...\n");
    shutdown();
    return 0;
}

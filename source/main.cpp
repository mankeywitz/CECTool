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
const std::string SERVER_URL = "http://192.168.1.247:8000";

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

    ret = httpcInit( HTTP_BUFFER_SIZE );
    if (R_FAILED(ret)) {
        printf("HTTP Init Failed: %lX\n", ret);
    }
    return ret;
}

void shutdown(void) {
    httpcExit();
    cecduExit();
}

int main(void) {
    Screens screens{};
    Result ret = init(screens);
    if (R_FAILED(ret)) {
        printf("Init Failed: %lX\n", ret);
        return 1;
    }
    cecToolDirectoryCheck();

    std::unique_ptr<StreetpassManager> sm = std::make_unique<StreetpassManager>();
    // Main menu loop; Start to exit
    bool showMenu = true;
    while (aptMainLoop()) {
        if (showMenu) {
            ClearScreens(screens);
            printf("CECTool\n\n");
            sm->ListBoxes();
            printf("\n\nMain Menu\n\n");

            printf("[A] Create\n");
            printf("[B] Delete\n");
            printf("[X] Export\n");
            printf("[Y] Import\n");
            printf("[L] Open\n");
            printf("[R] Test Server\n");

            printf("\nPress START to exit\n");
            showMenu = false;
        }
        
        gspWaitForVBlank();
        gfxSwapBuffers();
        hidScanInput();
        u32 down = hidKeysDown();

        if (down & KEY_A) {
            createMenu(screens, *sm);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_B) {
            deleteMenu(screens, *sm);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_X) {
            exportMenu(screens, *sm);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_Y) {
            importMenu(screens, *sm);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_L) {
            openMenu(screens, *sm);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_R) {
            pingServer(SERVER_URL);
            waitForInput();
            showMenu = true;
        } else if (down & KEY_START) {
            break;
        }
    }
    shutdown();
    return 0;
}

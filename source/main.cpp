#include <3ds.h>
#include <memory>
#include <sys/stat.h>

#include "common/util.hpp"
#include "create.hpp"
#include "delete.hpp"
#include "export.hpp"
#include "import.hpp"
#include "open.hpp"
#include "streetpass/StreetpassManager.hpp"
#include "tests/tests.hpp"

extern "C" {
#include "3ds/services/cecd.h"
}

using Streetpass::StreetpassManager;

int __stacksize__ = 64 * 1024;

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

    sdmcInit();
    if (R_FAILED(ret)) {
        printf("Sdmc Init Failed: %lX\n", ret);
        return ret;
    }

    ret = cecdInit();
    if (R_FAILED(ret)) {
        printf("Cecd Init Failed: %lX\n", ret);
    }
    return ret;
}

void shutdown(void) {
    cecdExit();
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
    hidScanInput();
    u32 down = hidKeysDown();
    while (aptMainLoop() && !(down & KEY_START)) {
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
            printf("[R] Tests\n");

            printf("\nPress START to exit\n");
            showMenu = false;
        }
        down = hidKeysDown();
        hidScanInput();

        if (down & KEY_A) {
            createMenu(screens, *sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_B) {
            deleteMenu(screens, *sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_X) {
            exportMenu(*sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_Y) {
            importMenu(screens, *sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_L) {
            openMenu(screens, *sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_R) {
            consoleSelect(&screens.bottom);
            printf("Testing...\n");
            Tests::RunAllTests();
            printf("Done!\n");
            consoleSelect(&screens.top);

            waitForInput();
            showMenu = true;
        }
    }
    shutdown();
    return 0;
}

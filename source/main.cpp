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

Screens screens{};

void cecToolDirectoryCheck(void) {
    // What happens if there is no sd card...?
    mkdir("/3ds/CECTool", 777);
    mkdir("/3ds/CECTool/export", 777);
    mkdir("/3ds/CECTool/export/streetpasses", 777);
    mkdir("/3ds/CECTool/import", 777);
    mkdir("/3ds/CECTool/import/streetpasses", 777);
    mkdir("/3ds/CECTool/tests", 777);
}

Result init(void) {
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
    Result ret = init();
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
            consoleClear();
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
            createMenu(*sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_B) {
            deleteMenu(*sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_X) {
            exportMenu(*sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_Y) {
            importMenu(*sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_L) {
            openMenu(*sm);
            down = hidKeysDown();
            showMenu = true;
        } else if (down & KEY_R) {
            printf("Testing...\n");
            Tests::RunAllTests();
            printf("Done!\n");
            waitForInput();
            showMenu = true;
        }
    }
    shutdown();
    return 0;
}

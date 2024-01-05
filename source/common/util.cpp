#include <3ds.h>
#include <cstdio>
#include "common/util.hpp"

void ClearScreen(PrintConsole* screen) {
    consoleSelect(screen);
    consoleClear();
}

void ClearScreens(Screens& screens) {
    ClearScreen(&screens.bottom);
    ClearScreen(&screens.top);
}

void SetColor(Color fgColor) {
    printf("\x1b[%dm", static_cast<int>(fgColor));
}

void SetColor(Color fgColor, Color bgColor) {
    printf("\x1b[%dm\x1b[%dm", static_cast<int>(fgColor), static_cast<int>(bgColor));
}

void waitForInput(void) {
    hidScanInput();
    while (aptMainLoop() && !hidKeysDown()) hidScanInput();
    hidScanInput();
}

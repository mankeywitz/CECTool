#include <3ds.h>
#include <cstdio>
#include "common/util.hpp"

void SetColor(Color fgColor) {
    printf("\x1b[%dm", static_cast<int>(fgColor));
}

void SetColor(Color fgColor, Color bgColor) {
    printf("\x1b[%dm\x1b[%dm", static_cast<int>(fgColor), static_cast<int>(bgColor));
}

void waitForInput() {
    while (aptMainLoop() && !hidKeysDown()) hidScanInput();
    hidScanInput();
}

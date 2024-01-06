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

std::string convertU8ArrayToString(const std::vector<u8>& bytes) {
    if(bytes.size() % 2 != 0) {
        printf("Malformed 16bit string");
        return "";
    }
    std::vector<u8> utf8Bytes(bytes.size());
    utf16_to_utf8(utf8Bytes.data(), (u16*)bytes.data(), bytes.size() / 2);

    utf8Bytes.push_back(0);
    return std::string((char *)utf8Bytes.data());
}

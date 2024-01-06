#pragma once

#include <3ds.h>
#include <string>
#include <vector>

constexpr int fgColorOffset = 30;
constexpr int bgColorOffset = 40;

enum class Color : int {
    Reset = 0,
    // Foreground Colors
    fgBlack = fgColorOffset + 0,
    fgRed = fgColorOffset + 1,
    fgGreen = fgColorOffset + 2,
    fgYellow = fgColorOffset + 3,
    fgBlue = fgColorOffset + 4,
    fgMagenta = fgColorOffset + 5,
    fgCyan = fgColorOffset + 6,
    fgWhite = fgColorOffset + 7,
    // Background Colors
    bgBlack = bgColorOffset + 0,
    bgRed = bgColorOffset + 1,
    bgGreen = bgColorOffset + 2,
    bgYellow = bgColorOffset + 3,
    bgBlue = bgColorOffset + 4,
    bgMagenta = bgColorOffset + 5,
    bgCyan = bgColorOffset + 6,
    bgWhite = bgColorOffset + 7
};

struct Screens {
    PrintConsole top;
    PrintConsole bottom;
};

void ClearScreen(PrintConsole* screen);
void ClearScreens(Screens& screens);

void SetColor(Color fgColor);
void SetColor(Color fgColor, Color bgColor);

void waitForInput(void);

std::string convertU8ArrayToString(const std::vector<u8>& bytes);

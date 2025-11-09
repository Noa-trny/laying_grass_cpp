#include "../include/ConsoleUtils.hpp"
#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#endif
#include <iostream>
#include <vector>

namespace ConsoleUtils {

const char* RESET_COLOR = "\x1b[0m";

static const std::vector<std::string> PLAYER_COLORS = {
    "\x1b[31m", // red
    "\x1b[34m", // blue
    "\x1b[32m", // green
    "\x1b[35m", // magenta
    "\x1b[33m", // yellow
    "\x1b[36m"  // cyan
};

std::string colorForPlayer(int id) {
    if (id <= 0) return std::string(RESET_COLOR);
    return PLAYER_COLORS[(id - 1) % static_cast<int>(PLAYER_COLORS.size())];
}

void clearConsole() {
#ifdef _WIN32
    // try ANSI sequence then cls
    std::cout << "\x1b[2J\x1b[H";
    std::system("cls");
#else
    std::cout << "\x1b[2J\x1b[H";
#endif
}

} // namespace ConsoleUtils


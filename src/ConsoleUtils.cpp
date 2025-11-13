#include "../include/ConsoleUtils.hpp"
#ifdef _WIN32
#include <windows.h>
#include <cstdlib>
#endif
#include <iostream>
#include <vector>
#include <unordered_map>

namespace ConsoleUtils {

const char* RESET_COLOR = "\x1b[0m";

static const std::vector<std::pair<std::string, std::string>> COLOR_OPTIONS = {
    {"Rouge", "\x1b[31m"},
    {"Bleu",  "\x1b[34m"},
    {"Vert",  "\x1b[32m"},
    {"Magenta", "\x1b[35m"},
    {"Jaune", "\x1b[33m"},
    {"Cyan",  "\x1b[36m"},
    {"Blanc", "\x1b[37m"},
    {"Gris",  "\x1b[90m"},
    {"Orange", "\x1b[38;5;214m"},
    {"Violet", "\x1b[38;5;93m"}
};

static std::unordered_map<int, std::string> customPlayerColors;

std::string colorForPlayer(int id) {
    if (id <= 0) return std::string(RESET_COLOR);
    auto it = customPlayerColors.find(id);
    if (it != customPlayerColors.end()) {
        return it->second;
    }
    const auto& fallback = COLOR_OPTIONS[(id - 1) % static_cast<int>(COLOR_OPTIONS.size())];
    return fallback.second;
}

void setPlayerColor(int id, const std::string& colorCode) {
    if (id > 0) {
        customPlayerColors[id] = colorCode;
    }
}

const std::vector<std::pair<std::string, std::string>>& colorOptions() {
    return COLOR_OPTIONS;
}

void clearConsole() {
#ifdef _WIN32
    // Tenter d'abord l'effacement ANSI puis recourir à cls si nécessaire.
    std::cout << "\x1b[2J\x1b[H";
    std::system("cls");
#else
    std::cout << "\x1b[2J\x1b[H";
#endif
}

} // espace de noms ConsoleUtils


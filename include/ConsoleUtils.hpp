#ifndef CONSOLEUTILS_HPP
#define CONSOLEUTILS_HPP

#include <string>

namespace ConsoleUtils {
    // Reset color escape sequence
    extern const char* RESET_COLOR;

    // Retourne la séquence d'échappement couleur pour un id de joueur (1..N)
    std::string colorForPlayer(int id);

    // Nettoie la console (cls sous Windows, ANSI sinon)
    void clearConsole();
}

#endif // CONSOLEUTILS_HPP


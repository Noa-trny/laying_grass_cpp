#ifndef CONSOLEUTILS_HPP
#define CONSOLEUTILS_HPP

#include <string>
#include <vector>
#include <utility>

namespace ConsoleUtils {
    // Séquence d'échappement permettant de réinitialiser les couleurs de la console.
    extern const char* RESET_COLOR;

    // Retourne la séquence d'échappement couleur pour un id de joueur (1..N).
    std::string colorForPlayer(int id);

    // Nettoie la console (cls sous Windows, ANSI sinon).
    void clearConsole();

    // Définit la couleur d'un joueur.
    void setPlayerColor(int id, const std::string& colorCode);

    // Renvoie la liste des couleurs disponibles (nom, code ANSI).
    const std::vector<std::pair<std::string, std::string>>& colorOptions();
}

#endif // CONSOLEUTILS_HPP


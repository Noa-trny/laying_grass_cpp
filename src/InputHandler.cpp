#include "../include/InputHandler.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::string trim(std::string value) {
    auto beginIt = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) { return std::isspace(ch) != 0; });
    auto endIt = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) { return std::isspace(ch) != 0; }).base();
    if (beginIt >= endIt) {
        return {};
    }
    return {beginIt, endIt};
}

std::string readLineOrThrow() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw std::runtime_error("Entree interrompue");
    }
    return line;
}

int readBoundedInt(int minValue, int maxValue) {
    while (true) {
        std::string line = trim(readLineOrThrow());
        if (line.empty()) {
            std::cout << "Veuillez entrer une valeur numerique." << '\n';
            continue;
        }
        bool isValid = true;
        for (char c : line) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                isValid = false;
                break;
            }
        }
        if (!isValid) {
            std::cout << "Entree invalide. Saisir un nombre entre " << minValue << " et " << maxValue << "." << '\n';
            continue;
        }
        int value = std::stoi(line);
        if (value < minValue || value > maxValue) {
            std::cout << "Valeur hors limites. Saisir un nombre entre " << minValue << " et " << maxValue << "." << '\n';
            continue;
        }
        return value;
    }
}

int parseCoordinateToken(const std::string& token, int maxValue) {
    if (token.empty()) return -1;

    bool allDigits = std::all_of(token.begin(), token.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });

    if (allDigits) {
        int value = std::stoi(token);
        if (value >= 1 && value <= maxValue) {
            return value - 1;
        }
        return -1;
    }

    bool allLetters = std::all_of(token.begin(), token.end(), [](unsigned char ch) {
        return std::isalpha(ch) != 0;
    });
    if (!allLetters) return -1;

    int index = 0;
    for (char ch : token) {
        char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        index = index * 26 + (upper - 'A' + 1);
    }
    if (index < 1 || index > maxValue) return -1;
    return index - 1;
}

int readCoordinate(const std::string& prompt, int maxValue) {
    while (true) {
        std::cout << prompt;
        std::string line = trim(readLineOrThrow());
        if (line.empty()) continue;
        int coord = parseCoordinateToken(line, maxValue);
        if (coord >= 0) {
            return coord;
        }
        std::cout << "Entree invalide. Utiliser des lettres (A, B, ... ou AA) ou un nombre entre 1 et " << maxValue << "." << '\n';
    }
}
}

Position InputHandler::getTilePosition(int boardSize) {
    int row = readCoordinate("Ligne (lettres ou chiffres) : ", boardSize);
    int col = readCoordinate("Colonne (lettres ou chiffres) : ", boardSize);
    return Position{row, col};
}

int InputHandler::getTileOrientation(const Tile&) {
    const std::vector<int> allowed = {0, 90, 180, 270};
    while (true) {
        std::cout << "Orientation (0, 90, 180, 270): ";
        std::string line = trim(readLineOrThrow());
        if (line.empty()) {
            continue;
        }
        std::stringstream ss(line);
        int value = 0;
        if (ss >> value) {
            // consommer les espaces restants puis vérifier qu'il n'y a plus de caractères
            ss >> std::ws;
            if (ss.eof()) {
                if (std::find(allowed.begin(), allowed.end(), value) != allowed.end()) {
                    return value;
                }
            }
        }
        std::cout << "Entrée invalide. Choisir parmi 0, 90, 180 ou 270." << '\n';
    }
}

int InputHandler::getTileSelection(const std::vector<Tile>& options) {
    if (options.empty()) {
        return -1;
    }
    std::cout << "Choisir une tuile entre 1 et " << options.size() << ": ";
    return readBoundedInt(1, static_cast<int>(options.size())) - 1;
}

bool InputHandler::confirmAction(const std::string& prompt) {
    while (true) {
        std::cout << prompt << " (o/n): ";
        std::string line = trim(readLineOrThrow());
        if (line.empty()) {
            continue;
        }
        char value = static_cast<char>(std::tolower(static_cast<unsigned char>(line.front())));
        if (value == 'o' || value == 'y') {
            return true;
        }
        if (value == 'n') {
            return false;
        }
        std::cout << "Repondre par o ou n." << std::endl;
        std::cout.flush();
    }
}

int InputHandler::getMenuChoice(const std::vector<std::string>& options) {
    if (options.empty()) {
        throw std::invalid_argument("Liste d'options vide");
    }
    while (true) {
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << i + 1 << ". " << options[i] << std::endl;
        }
        std::cout << "Choix: ";
        int index = readBoundedInt(1, static_cast<int>(options.size()));
        return index;
    }
}

Position InputHandler::getStonePosition(int boardSize) {
    int row = readCoordinate("Ligne de la pierre : ", boardSize);
    int col = readCoordinate("Colonne de la pierre : ", boardSize);
    return Position{row, col};
}

int InputHandler::getRobberyTarget(const std::vector<int>& availablePlayers) {
    if (availablePlayers.empty()) {
        return -1;
    }
    std::cout << "Choisir une cible: ";
    int choice = readBoundedInt(1, static_cast<int>(availablePlayers.size()));
    return availablePlayers[choice - 1];
}

std::string InputHandler::getPlayerName(int playerId) {
    std::cout << "Nom du joueur " << playerId << " (laisser vide pour indiquer Joueur " << playerId << "): ";
    std::string line = trim(readLineOrThrow());
    if (line.empty()) {
        return "Joueur " + std::to_string(playerId);
    }
    return line;
}

int InputHandler::getPlayerCount() {
    std::cout << "Nombre de joueurs (" << MIN_PLAYERS << "-" << MAX_PLAYERS << "): ";
    return readBoundedInt(MIN_PLAYERS, MAX_PLAYERS);
}

int InputHandler::chooseColor(const std::vector<std::string>& colorNames) {
    if (colorNames.empty()) return -1;
    std::cout << "Choisir une couleur :" << std::endl;
    for (size_t i = 0; i < colorNames.size(); ++i) {
        std::cout << i + 1 << ". " << colorNames[i] << std::endl;
    }
    std::cout << "Choix: ";
    return readBoundedInt(1, static_cast<int>(colorNames.size())) - 1;
}

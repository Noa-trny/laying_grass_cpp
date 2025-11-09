#include "../include/Game.hpp"
#include "../include/TileParser.hpp"
#include "../include/GameUtils.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#endif

Game::Game(int numPlayers)
    : board(numPlayers),
      players(),
      currentRound(0),
      currentPlayerIndex(0),
      running(true) {
    initializePlayers(numPlayers);
    // Charger les tuiles au démarrage depuis data/tiles.json
    try {
        loadTilesFromJson();
    } catch (const std::exception& ex) {
        std::cerr << "Erreur chargement tiles.json: " << ex.what() << std::endl;
    }
}

void Game::initializePlayers(int numPlayers) {
    if (numPlayers <= 0) {
        throw std::invalid_argument("Nombre de joueurs invalide");
    }
    players.clear();
    players.reserve(numPlayers);
    for (int i = 0; i < numPlayers; ++i) {
        std::string name = InputHandler::getPlayerName(i + 1);
        if (name.empty()) {
            name = "Joueur " + std::to_string(i + 1);
        }
        players.emplace_back(i + 1, name, Position{0, 0});
    }
}

void Game::showTurnIntro(const Player& player) const {
    std::cout << std::endl;
    std::cout << "Tour " << currentRound + 1 << " | Joueur " << player.getId() << " : " << player.getName() << std::endl;
}

void Game::handlePlayerTurn(Player& player) {
    const std::vector<std::string> options = {
        "Jouer une action",
        "Afficher le plateau",
        "Afficher les joueurs",
        "Afficher 3 tuiles aleatoires",
        "Terminer la partie"
    };
    bool turnFinished = false;
    while (!turnFinished && running) {
        int choice = InputHandler::getMenuChoice(options);
        switch (choice) {
            case 1:
                std::cout << player.getName() << " place une tuile fictive." << std::endl;
                player.incrementGrassTiles();
                turnFinished = true;
                break;
            case 2:
                showBoard();
                break;
            case 3:
                showPlayers();
                break;
            case 4: {
                // afficher 3 tuiles aléatoires
                auto tiles = getRandomTiles(3);
                showTiles(tiles);
                break;
            }
            case 5:
                if (InputHandler::confirmAction("Confirmer la fin de partie ?")) {
                    running = false;
                    turnFinished = true;
                }
                break;
            default:
                break;
        }
    }
}

void Game::advanceTurn() {
    if (players.empty()) {
        return;
    }
    ++currentPlayerIndex;
    if (currentPlayerIndex >= static_cast<int>(players.size())) {
        currentPlayerIndex = 0;
        ++currentRound;
    }
}

void Game::showBoard() const {
    int size = board.getSize();
    std::cout << "Plateau " << size << " x " << size << std::endl;
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            int cell = board.getCell(row, col);
            std::cout << (cell == 0 ? '.' : '#');
        }
        std::cout << '\n';
    }
}

void Game::showPlayers() const {
    std::cout << "Participants:" << std::endl;
    for (const auto& player : players) {
        std::cout << "#" << player.getId() << " " << player.getName() << " | Tuiles posées: " << player.getGrassTilesPlaced() << std::endl;
    }
}

void Game::showSummary() const {
    std::cout << std::endl;
    std::cout << "Fin de partie après " << currentRound + 1 << " tours complets." << std::endl;
    showPlayers();
}

void Game::loadTilesFromJson(const std::string& filepath) {
    // Délégation au parser centralisé
    availableTiles.clear();
    availableTiles = TileParser::loadTilesFromJson(filepath);
}

void Game::loadTilesFromJson() {
    // essaie plusieurs chemins plausibles
    std::vector<std::string> candidates = {
        "data/tiles.json",
        "./data/tiles.json",
        "../data/tiles.json",
        "./tiles.json",
        "tiles.json"
    };

#ifdef _WIN32
    // essayer aussi par rapport au répertoire de l'exécutable
    char buf[MAX_PATH];
    if (GetModuleFileNameA(nullptr, buf, MAX_PATH) > 0) {
        std::filesystem::path exePath(buf);
        auto dir = exePath.parent_path();
        candidates.push_back((dir / "data" / "tiles.json").string());
        candidates.push_back((dir / ".." / "data" / "tiles.json").lexically_normal().string());
    }
#endif

    for (const auto& c : candidates) {
        try {
            availableTiles = TileParser::loadTilesFromJson(c);
            return;
        } catch (...) {
            // ignorer et essayer le suivant
        }
    }
    throw std::runtime_error("Impossible de trouver data/tiles.json dans les chemins candidats");
}

std::vector<Tile> Game::getRandomTiles(int n) const {
    return GameUtils::pickRandomTiles(availableTiles, n);
}

void Game::showTile(const Tile& tile) const {
    GameUtils::showTileConsole(tile);
}

void Game::showTiles(const std::vector<Tile>& tiles) const {
    GameUtils::showTilesConsole(tiles);
}

void Game::run() {
    if (players.empty()) {
        std::cout << "Aucun joueur enregistré." << std::endl;
        return;
    }
    while (running) {
        Player& current = players[currentPlayerIndex];
        showTurnIntro(current);
        handlePlayerTurn(current);
        if (running) {
            advanceTurn();
        }
    }
    showSummary();
}

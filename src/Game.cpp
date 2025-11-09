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
      running(true),
      tileQueue(numPlayers, std::vector<Tile>{}),
      currentTile(-1, {}),
      hasCurrentTile(false) {
    initializePlayers(numPlayers);
    // Charger les tuiles au demarrage depuis data/tiles.json
    try {
        loadTilesFromJson();
    } catch (const std::exception& ex) {
        std::cerr << "Erreur chargement tiles.json: " << ex.what() << std::endl;
    }
    // Initialiser la queue avec les tuiles disponibles si presentes
    if (!availableTiles.empty()) {
        tileQueue = Queue(static_cast<int>(players.size()), availableTiles);
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

void Game::drawNextTile() {
    if (!tileQueue.hasNext()) {
        hasCurrentTile = false;
        return;
    }
    currentTile = tileQueue.getNext();
    hasCurrentTile = currentTile.isValid();
    if (hasCurrentTile) {
        std::cout << "Tuile tiree:" << std::endl;
        showTile(currentTile);
    } else {
        std::cout << "Aucune tuile disponible." << std::endl;
    }
}

void Game::rotateCurrentTile() {
    if (!hasCurrentTile) return;
    currentTile = currentTile.rotate90();
    std::cout << "Tuile apres rotation:" << std::endl;
    showTile(currentTile);
}

void Game::placeCurrentTile(Player& player) {
    if (!hasCurrentTile) {
        std::cout << "Pas de tuile a placer." << std::endl;
        return;
    }
    Position pos = InputHandler::getTilePosition();
    int orientation = InputHandler::getTileOrientation(currentTile);
    Tile toPlace = currentTile;
    if (orientation == 90) toPlace = toPlace.rotate90();
    else if (orientation == 180) toPlace = toPlace.rotate180();
    else if (orientation == 270) toPlace = toPlace.rotate270();

    if (!board.canPlaceTile(toPlace, pos, player.getId())) {
        std::cout << "Placement invalide (limites, collision ou non-connexite)." << std::endl;
        return;
    }
    board.placeTile(toPlace, pos, player.getId());
    hasCurrentTile = false;
    std::cout << "Tuile placee." << std::endl;
}

void Game::exchangeCurrentTile(Player& player) {
    if (!hasCurrentTile) {
        std::cout << "Pas de tuile a echanger." << std::endl;
        return;
    }
    if (player.getExchangeCoupons() <= 0) {
        std::cout << "Aucun coupon d'echange." << std::endl;
        return;
    }
    auto preview = tileQueue.peekNext(EXCHANGE_PREVIEW_SIZE);
    if (preview.empty()) {
        std::cout << "Pas assez de tuiles en file pour echanger." << std::endl;
        return;
    }
    std::cout << "Apercu des prochaines tuiles:" << std::endl;
    showTiles(preview);
    std::cout << "Choisissez une tuile (1-" << preview.size() << ") a prendre: ";
    int idx = InputHandler::getTileSelection(preview);
    Tile chosen = tileQueue.exchangeTile(idx);
    if (!chosen.isValid()) {
        std::cout << "Echange impossible." << std::endl;
        return;
    }
    tileQueue.returnToQueue(currentTile);
    currentTile = chosen;
    player.useExchangeCoupon();
    std::cout << "Tuile echangee. Nouvelle tuile:" << std::endl;
    showTile(currentTile);
}

void Game::handlePlayerTurn(Player& player) {
    if (!hasCurrentTile) {
        drawNextTile();
    }
    const std::vector<std::string> options = {
        "Placer la tuile",
        "Pivoter la tuile (90 deg)",
        "Afficher le plateau",
        "Afficher les joueurs",
        "Apercu de 3 tuiles aleatoires",
        "Echanger la tuile (si coupon)",
        "Terminer la partie"
    };
    bool turnFinished = false;
    while (!turnFinished && running) {
        if (hasCurrentTile) {
            std::cout << "Tuile courante:" << std::endl; showTile(currentTile);
        }
        int choice = InputHandler::getMenuChoice(options);
        switch (choice) {
            case 1:
                placeCurrentTile(player);
                turnFinished = true;
                break;
            case 2:
                rotateCurrentTile();
                break;
            case 3:
                showBoard();
                break;
            case 4:
                showPlayers();
                break;
            case 5: {
                auto tiles = getRandomTiles(3);
                showTiles(tiles);
                break;
            }
            case 6:
                exchangeCurrentTile(player);
                break;
            case 7:
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
        std::cout << "#" << player.getId() << " " << player.getName() << " | Tuiles posees: " << player.getGrassTilesPlaced() << std::endl;
    }
}

void Game::showSummary() const {
    std::cout << std::endl;
    std::cout << "Fin de partie apres " << currentRound + 1 << " tours complets." << std::endl;
    showPlayers();
}

void Game::loadTilesFromJson(const std::string& filepath) {
    availableTiles.clear();
    availableTiles = TileParser::loadTilesFromJson(filepath);
}

void Game::loadTilesFromJson() {
    std::vector<std::string> candidates = {
        "data/tiles.json",
        "./data/tiles.json",
        "../data/tiles.json",
        "./tiles.json",
        "tiles.json"
    };
#ifdef _WIN32
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
        }
    }
    throw std::runtime_error("Impossible de trouver data/tiles.json dans les chemins candidats");
}

std::vector<Tile> Game::getRandomTiles(int n) const { return GameUtils::pickRandomTiles(availableTiles, n); }
void Game::showTile(const Tile& tile) const { GameUtils::showTileConsole(tile); }
void Game::showTiles(const std::vector<Tile>& tiles) const { GameUtils::showTilesConsole(tiles); }

void Game::run() {
    if (players.empty()) {
        std::cout << "Aucun joueur enregistre." << std::endl;
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

#include "../include/Game.hpp"
#include "../include/TileParser.hpp"
#include "../include/GameUtils.hpp"
#include "../include/ConsoleUtils.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
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
    // Activer séquences ANSI sous Windows si possible
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
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

void Game::handlePlayerTurn(Player& player) {
    // Simplified menu: jouer action ou terminer
    const std::vector<std::string> options = {
        "Jouer une action",
        "Terminer la partie"
    };

    bool turnFinished = false;

    // Nettoyer la console et afficher l'essentiel (plateau + joueurs) — sans tuiles tant que l'on n'a pas choisi d'agir
    ConsoleUtils::clearConsole();
    showTurnIntro(player);
    showBoard();
    showPlayers();

    while (!turnFinished && running) {
        int choice = InputHandler::getMenuChoice(options);
        switch (choice) {
            case 1: {
                // Lorsque le joueur décide de jouer, tirer et afficher les 3 tuiles
                auto tiles = getRandomTiles(3);

                ConsoleUtils::clearConsole();
                showTurnIntro(player);
                showBoard();
                showPlayers();

                if (!tiles.empty()) {
                    std::string col = ConsoleUtils::colorForPlayer(player.getId());
                    // afficher la tuile courante comme (1) et les prochaines comme (2),(3)
                    GameUtils::showTileConsoleColored(tiles[0], col, ConsoleUtils::RESET_COLOR, 1);
                    if (tiles.size() > 1) {
                        std::vector<Tile> nextTiles(tiles.begin() + 1, tiles.end());
                        std::vector<int> nums;
                        nums.reserve(nextTiles.size());
                        for (int i = 0; i < static_cast<int>(nextTiles.size()); ++i) nums.push_back(i + 2); // 2..n
                        GameUtils::showTilesConsoleColored(nextTiles, col, ConsoleUtils::RESET_COLOR, nums);
                    }
                }

                // Jouer une action : sélectionner parmi les 3 tuiles affichées et poser
                if (tiles.empty()) {
                    std::cout << "Aucune tuile disponible." << '\n';
                    turnFinished = true;
                    break;
                }
                bool placed = false;
                while (!placed) {
                    int sel = InputHandler::getTileSelection(tiles);
                    if (sel < 0 || sel >= static_cast<int>(tiles.size())) {
                        std::cout << "Selection invalide." << std::endl;
                        if (!InputHandler::confirmAction("Voulez-vous reessayer la selection ?")) break;
                        continue;
                    }
                    const Tile& chosenRef = tiles[sel];
                    Tile chosen = chosenRef; // copy for orientation
                    // choisir orientation
                    int orientation = InputHandler::getTileOrientation(chosen);
                    Tile oriented = chosen;
                    if (orientation == 90) oriented = chosen.rotate90();
                    else if (orientation == 180) oriented = chosen.rotate180();
                    else if (orientation == 270) oriented = chosen.rotate270();

                    // interactive positioning with overlay preview
                    bool changeTileRequested = false;
                    std::string col = ConsoleUtils::colorForPlayer(player.getId());
                    while (!changeTileRequested && !placed) {
                        // ask for candidate position (letters)
                        Position candidate = InputHandler::getTilePosition();
                        // show overlay preview
                        ConsoleUtils::clearConsole();
                        showTurnIntro(player);
                        GameUtils::showBoardWithOverlay(board, oriented, candidate, col, ConsoleUtils::RESET_COLOR);
                        showPlayers();

                        if (!InputHandler::confirmAction("Confirmer le placement ici ?")) {
                            // ask if change tile
                            if (InputHandler::confirmAction("Voulez-vous changer de tuile ?")) {
                                changeTileRequested = true;
                                break; // go back to tile selection
                            }
                            // otherwise clear preview and loop to pick another candidate
                            ConsoleUtils::clearConsole();
                            showTurnIntro(player);
                            showBoard();
                            showPlayers();
                            continue;
                        }

                        // l'utilisateur a confirme; nettoyer l'ecran avant d'afficher le resultat pour eviter caracteres residuels
                        ConsoleUtils::clearConsole();

                        // user confirmed position, validate
                        if (board.canPlaceTile(oriented, candidate, player.getId())) {
                            board.placeTile(oriented, candidate, player.getId());
                            player.incrementGrassTiles();
                            // afficher plateau mis a jour proprement
                            showTurnIntro(player);
                            showBoard();
                            showPlayers();
                            std::cout << "Tuile posee avec succes." << '\n';
                            placed = true;
                            break;
                        } else {
                            // afficher message d'erreur sur un ecran propre
                            std::cout << "Placement invalide a la position choisie." << '\n';
                            if (!InputHandler::confirmAction("Voulez-vous reessayer la position ?")) {
                                if (InputHandler::confirmAction("Voulez-vous changer de tuile ?")) {
                                    changeTileRequested = true;
                                    break;
                                }
                                break;
                            }
                            // else (user veut reessayer la position) : nettoyer l'ecran avant de re-previsualiser
                            ConsoleUtils::clearConsole();
                            showTurnIntro(player);
                            showBoard();
                            showPlayers();
                            // puis continuer la boucle pour redemander la position
                        }
                    }
                    if (changeTileRequested) {
                        // re-display tile choices
                        ConsoleUtils::clearConsole();
                        showTurnIntro(player);
                        showBoard();
                        showPlayers();
                        std::string col2 = ConsoleUtils::colorForPlayer(player.getId());
                        GameUtils::showTileConsoleColored(tiles[0], col2, ConsoleUtils::RESET_COLOR, 1);
                        if (tiles.size() > 1) {
                            std::vector<Tile> nextTiles(tiles.begin() + 1, tiles.end());
                            std::vector<int> nums;
                            nums.reserve(nextTiles.size());
                            for (int i = 0; i < static_cast<int>(nextTiles.size()); ++i) nums.push_back(i + 2);
                            GameUtils::showTilesConsoleColored(nextTiles, col2, ConsoleUtils::RESET_COLOR, nums);
                        }
                        continue; // go back to tile selection loop
                    }
                }
                // après pose, afficher plateau mis à jour
                ConsoleUtils::clearConsole();
                showTurnIntro(player);
                showBoard();
                showPlayers();
                turnFinished = true;
                break;
            }
            case 2:
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

void Game::run() {
    if (players.empty()) {
        std::cout << "Aucun joueur enregistré." << std::endl;
        return;
    }
    while (running) {
        Player& current = players[currentPlayerIndex];
        handlePlayerTurn(current);
        if (running) {
            advanceTurn();
            // clear screen between turns to aerer le jeu
            ConsoleUtils::clearConsole();
         }
     }
    // Résumé final déplacé dans GameUI
}

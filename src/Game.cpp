#include "../include/Game.hpp"
#include "../include/GameUtils.hpp"
#include "../include/ConsoleUtils.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <random>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif

Game::Game(int numPlayers)
    : board(numPlayers),
      players(),
      tileQueue(numPlayers),
      validator(&board),
      bonusManager(&board),
      currentRound(0),
      currentPlayerIndex(0),
      running(true),
      gameFinished(false) {
    initializePlayers(numPlayers);
    board.initializeBonusSquares(numPlayers);
    initializeStartingTiles();
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
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
        players[i].addExchangeCoupon();
    }
}

void Game::initializeStartingTiles() {
    int size = board.getSize();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, size - 3);
    
    std::vector<Position> usedPositions;
    
    for (auto& player : players) {
        Position startPos;
        bool valid = false;
        int attempts = 0;
        
        while (!valid && attempts < 100) {
            startPos = Position(dis(gen), dis(gen));
            valid = true;
            
            for (const auto& used : usedPositions) {
                int dist = std::abs(startPos.row - used.row) + std::abs(startPos.col - used.col);
                if (dist < 5) {
                    valid = false;
                    break;
                }
            }
            attempts++;
        }
        
        if (valid) {
            player.setStartPosition(startPos);
            Tile startingTile(0, std::vector<std::vector<bool>>{{true}});
            board.placeTile(startingTile, startPos, player.getId());
            board.addTerritory(player.getId(), startPos);
            player.incrementGrassTiles();
            usedPositions.push_back(startPos);
        }
    }
}

void Game::handlePlayerTurn(Player& player) {
    if (!tileQueue.hasNext()) {
        std::cout << "Plus de tuiles disponibles." << std::endl;
        return;
    }

    ConsoleUtils::clearConsole();
    showTurnIntro(player);
    showBoard();
    showPlayers();

    std::vector<std::string> options = {"Prendre la tuile suivante", "Echanger la tuile"};
    if (player.getExchangeCoupons() == 0) {
        options = {"Prendre la tuile suivante"};
    }

    int choice = InputHandler::getMenuChoice(options);
    Tile currentTile;

    if (choice == 2 && player.getExchangeCoupons() > 0) {
        std::vector<Tile> preview = tileQueue.peekNext(EXCHANGE_PREVIEW_SIZE);
        if (preview.empty()) {
            std::cout << "Pas assez de tuiles pour l'echange." << std::endl;
            currentTile = tileQueue.getNext();
        } else {
            std::cout << "Tuiles disponibles (choisir 1-" << preview.size() << "):" << std::endl;
            std::string col = ConsoleUtils::colorForPlayer(player.getId());
            for (size_t i = 0; i < preview.size(); ++i) {
                std::cout << "(" << (i + 1) << ") ";
                GameUtils::showTileConsoleColored(preview[i], col, ConsoleUtils::RESET_COLOR, static_cast<int>(i + 1));
            }
            int sel = InputHandler::getTileSelection(preview);
            if (sel >= 0 && sel < static_cast<int>(preview.size())) {
                currentTile = tileQueue.exchangeTile(sel);
                player.useExchangeCoupon();
            } else {
                currentTile = tileQueue.getNext();
            }
        }
    } else {
        currentTile = tileQueue.getNext();
    }

    bool placed = false;
    bool tileDiscarded = false;

    while (!placed && !tileDiscarded) {
        ConsoleUtils::clearConsole();
        showTurnIntro(player);
        showBoard();
        showPlayers();

        std::string col = ConsoleUtils::colorForPlayer(player.getId());
        std::cout << "Tuile courante:" << std::endl;
        GameUtils::showTileConsoleColored(currentTile, col, ConsoleUtils::RESET_COLOR);

        std::vector<std::string> tileOptions = {"Tourner (R)", "Retourner (F)", "Placer (P)", "Abandonner"};
        int tileChoice = InputHandler::getMenuChoice(tileOptions);
        
        Tile oriented = currentTile;

        if (tileChoice == 1) {
            int rotation = InputHandler::getTileOrientation(oriented);
            if (rotation == 90) oriented = currentTile.rotate90();
            else if (rotation == 180) oriented = currentTile.rotate180();
            else if (rotation == 270) oriented = currentTile.rotate270();
            currentTile = oriented;
            continue;
        } else if (tileChoice == 2) {
            oriented = currentTile.flipHorizontal();
            currentTile = oriented;
            continue;
        } else if (tileChoice == 4) {
            tileQueue.returnToQueue(currentTile);
            tileDiscarded = true;
            std::cout << "Tuile abandonnee." << std::endl;
            break;
        }

        Position candidate = InputHandler::getTilePosition();
        ConsoleUtils::clearConsole();
        showTurnIntro(player);
        GameUtils::showBoardWithOverlay(board, oriented, candidate, col, ConsoleUtils::RESET_COLOR);
        showPlayers();

        if (!InputHandler::confirmAction("Confirmer le placement ici ?")) {
            continue;
        }

        bool isFirstTile = player.getGrassTilesPlaced() == 1;
        if (validator.isValidPlacement(oriented, candidate, player, isFirstTile)) {
            board.placeTile(oriented, candidate, player.getId());
            player.incrementGrassTiles();

            auto shape = oriented.getShape();
            for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
                for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
                    if (shape[r][c]) {
                        Position tilePos(candidate.row + r, candidate.col + c);
                        board.addTerritory(player.getId(), tilePos);
                        
                        if (board.checkBonusCapture(tilePos.row, tilePos.col, player.getId())) {
                            bonusManager.processBonusCapture(tilePos.row, tilePos.col, player.getId(), player, players);
                        }
                    }
                }
            }

            ConsoleUtils::clearConsole();
            showTurnIntro(player);
            showBoard();
            showPlayers();
            std::cout << "Tuile posee avec succes." << std::endl;
            placed = true;
        } else {
            std::cout << "Placement invalide." << std::endl;
            if (!InputHandler::confirmAction("Voulez-vous reessayer ?")) {
                tileQueue.returnToQueue(currentTile);
                tileDiscarded = true;
            }
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


void Game::run() {
    if (players.empty()) {
        std::cout << "Aucun joueur enregistré." << std::endl;
        return;
    }

    while (running && currentRound < TILES_PER_PLAYER) {
        Player& current = players[currentPlayerIndex];
        handlePlayerTurn(current);
        if (running) {
            advanceTurn();
            ConsoleUtils::clearConsole();
        }
    }

    if (!gameFinished) {
        processFinalPhase();
        determineWinner();
        showSummary();
    }
}

void Game::processFinalPhase() {
    ConsoleUtils::clearConsole();
    std::cout << "=== PHASE FINALE ===" << std::endl;
    std::cout << "Vous pouvez acheter des tuiles 1x1 avec vos coupons d'echange." << std::endl;

    for (auto& player : players) {
        if (player.getExchangeCoupons() > 0) {
            std::cout << std::endl << player.getName() << " a " << player.getExchangeCoupons() << " coupon(s)." << std::endl;
            while (player.getExchangeCoupons() > 0) {
                if (InputHandler::confirmAction("Voulez-vous acheter une tuile 1x1 ?")) {
                    Tile smallTile(0, std::vector<std::vector<bool>>{{true}});
                    Position pos = InputHandler::getTilePosition();
                    
                    bool isFirstTile = (player.getGrassTilesPlaced() == 1);
                    if (validator.isValidPlacement(smallTile, pos, player, isFirstTile)) {
                        board.placeTile(smallTile, pos, player.getId());
                        board.addTerritory(player.getId(), pos);
                        player.incrementGrassTiles();
                        player.useExchangeCoupon();
                        std::cout << "Tuile 1x1 placee." << std::endl;
                    } else {
                        std::cout << "Placement invalide." << std::endl;
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }
}

void Game::determineWinner() const {
    int maxSquareSize = 0;
    std::vector<int> winners;
    
    for (const auto& player : players) {
        SquareResult result = Algorithms::findLargestSquare(board, player.getId());
        if (result.size > maxSquareSize) {
            maxSquareSize = result.size;
            winners.clear();
            winners.push_back(player.getId());
        } else if (result.size == maxSquareSize && maxSquareSize > 0) {
            winners.push_back(player.getId());
        }
    }
    
    if (winners.empty() || maxSquareSize == 0) {
        int maxTiles = 0;
        winners.clear();
        for (const auto& player : players) {
            if (player.getGrassTilesPlaced() > maxTiles) {
                maxTiles = player.getGrassTilesPlaced();
                winners.clear();
                winners.push_back(player.getId());
            } else if (player.getGrassTilesPlaced() == maxTiles) {
                winners.push_back(player.getId());
            }
        }
    }
    
    ConsoleUtils::clearConsole();
    std::cout << "=== RESULTATS ===" << std::endl;
    for (int winnerId : winners) {
        for (const auto& player : players) {
            if (player.getId() == winnerId) {
                std::cout << "Gagnant: " << player.getName() << std::endl;
                break;
            }
        }
    }
}

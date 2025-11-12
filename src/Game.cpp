#include "../include/Game.hpp"
#include "../include/GameUtils.hpp"
#include "../include/ConsoleUtils.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <random>
#include <algorithm>
#include <set>
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
      gameFinished(false),
      maxRounds(0) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif
    initializePlayers(numPlayers);
    board.initializeBonusSquares(numPlayers);
    initializeStartingTiles();
    int totalTiles = tileQueue.getTotalTiles();
    if (players.empty()) {
        maxRounds = 0;
    } else {
        maxRounds = std::min(9, std::max(1, totalTiles / static_cast<int>(players.size())));
    }
}

void Game::initializePlayers(int numPlayers) {
    if (numPlayers <= 0) {
        throw std::invalid_argument("Nombre de joueurs invalide");
    }
    players.clear();
    players.reserve(numPlayers);
    const auto& colorChoices = ConsoleUtils::colorOptions();
    std::vector<bool> colorUsed(colorChoices.size(), false);

    for (int i = 0; i < numPlayers; ++i) {
        std::string name = InputHandler::getPlayerName(i + 1);
        if (name.empty()) {
            name = "Joueur " + std::to_string(i + 1);
        }

        std::vector<std::string> availableNames;
        std::vector<int> availableIndices;
        for (size_t idx = 0; idx < colorChoices.size(); ++idx) {
            if (!colorUsed[idx]) {
                availableNames.push_back(colorChoices[idx].first);
                availableIndices.push_back(static_cast<int>(idx));
            }
        }

        int chosenIndex = 0;
        if (!availableIndices.empty()) {
            int selection = InputHandler::chooseColor(availableNames);
            chosenIndex = availableIndices[selection];
            colorUsed[chosenIndex] = true;
        } else {
            chosenIndex = i % static_cast<int>(colorChoices.size());
        }

        Player player(i + 1, name, Position{0, 0});
        player.setColor(colorChoices[chosenIndex].first, colorChoices[chosenIndex].second);
        player.addExchangeCoupon();
        players.push_back(player);
        ConsoleUtils::setPlayerColor(player.getId(), colorChoices[chosenIndex].second);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(players.begin(), players.end(), gen);
    currentPlayerIndex = 0;

    std::cout << "Ordre de jeu initialise: ";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << players[i].getName();
        if (i + 1 < players.size()) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

void Game::initializeStartingTiles() {
    ConsoleUtils::clearConsole();
    std::cout << "=== Placement des tuiles de depart ===" << std::endl;
    Tile startingTile(0, std::vector<std::vector<bool>>{{true}});

    for (auto& player : players) {
        bool placed = false;
        while (!placed) {
            showBoard();
            std::cout << "Joueur " << player.getId() << " - " << ConsoleUtils::colorForPlayer(player.getId())
                      << player.getName() << ConsoleUtils::RESET_COLOR << ", choisissez une case de depart." << std::endl;
            Position pos = InputHandler::getTilePosition(board.getSize());
            if (!board.isInside(pos)) {
                std::cout << "Position hors du plateau." << std::endl;
                continue;
            }
            if (board.getCell(pos.row, pos.col) != 0 || board.hasStone(pos.row, pos.col)) {
                std::cout << "Case deja occupee." << std::endl;
                continue;
            }
            board.placeTile(startingTile, pos, player.getId());
            board.addTerritory(player.getId(), pos);
            player.setStartPosition(pos);
            player.addGrassSquares(1);
            placed = true;
            ConsoleUtils::clearConsole();
        }
    }
    showBoard();
    std::cout << "Tuiles de depart placees. Lancement de la partie..." << std::endl;
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
    std::cout << "Tuiles restantes dans la file: " << tileQueue.getRemainingCount() << std::endl;

    Tile currentTile;
    bool tileReady = false;
    while (!tileReady) {
        std::vector<std::string> options;
        options.push_back("Prendre la tuile suivante");
        int exchangeIndex = -1;
        int removeStoneIndex = -1;
        if (player.getExchangeCoupons() > 0) {
            exchangeIndex = static_cast<int>(options.size());
            options.push_back("Echanger contre les 5 prochaines tuiles (1 coupon)");
            removeStoneIndex = static_cast<int>(options.size());
            options.push_back("Retirer une pierre (1 coupon)");
        }

        int choice = InputHandler::getMenuChoice(options) - 1;

        if (choice == 0) {
            currentTile = tileQueue.getNext();
            tileReady = true;
        } else if (exchangeIndex != -1 && choice == exchangeIndex) {
            std::vector<Tile> preview = tileQueue.peekNext(EXCHANGE_PREVIEW_SIZE);
            if (preview.empty()) {
                std::cout << "Pas assez de tuiles pour l'echange." << std::endl;
                continue;
            }
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
                tileReady = true;
            } else {
                std::cout << "Selection invalide. Aucun echange effectue." << std::endl;
            }
        } else if (removeStoneIndex != -1 && choice == removeStoneIndex) {
            if (player.getExchangeCoupons() <= 0) {
                std::cout << "Aucun coupon disponible." << std::endl;
                continue;
            }
            Position stonePos = InputHandler::getStonePosition(board.getSize());
            if (!board.hasStone(stonePos.row, stonePos.col)) {
                std::cout << "Aucune pierre a cette position." << std::endl;
                continue;
            }
            if (player.useExchangeCoupon()) {
                board.removeStone(stonePos, true);
                std::cout << "Pierre retiree." << std::endl;
                ConsoleUtils::clearConsole();
                showTurnIntro(player);
                showBoard();
                showPlayers();
                std::cout << "Tuiles restantes dans la file: " << tileQueue.getRemainingCount() << std::endl;
            }
        } else {
            std::cout << "Choix invalide." << std::endl;
        }
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

        std::vector<std::string> tileOptions = {
            "Tourner la tuile",
            "Retourner horizontalement",
            "Retourner verticalement",
            "Placer la tuile",
            "Abandonner (defausser)"
        };
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
            currentTile = currentTile.flipHorizontal();
            continue;
        } else if (tileChoice == 3) {
            currentTile = currentTile.flipVertical();
            continue;
        } else if (tileChoice == 5) {
            tileDiscarded = true;
            std::cout << "Tuile abandonnee et retiree du jeu." << std::endl;
            break;
        }

        Position candidate = InputHandler::getTilePosition(board.getSize());
        ConsoleUtils::clearConsole();
        showTurnIntro(player);
        GameUtils::showBoardWithOverlay(board, currentTile, candidate, col, ConsoleUtils::RESET_COLOR);
        showPlayers();

        if (!InputHandler::confirmAction("Confirmer le placement ici ?")) {
            continue;
        }

        bool isFirstTile = (player.getGrassSquaresOwned() == 1);
        if (validator.isValidPlacement(currentTile, candidate, player, isFirstTile)) {
            board.placeTile(currentTile, candidate, player.getId());

            auto shape = currentTile.getShape();
            std::set<Position> placedPositions;
            for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
                for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
                    if (shape[r][c]) {
                        Position tilePos(candidate.row + r, candidate.col + c);
                        board.addTerritory(player.getId(), tilePos);
                        placedPositions.insert(tilePos);
                    }
                }
            }
            
            player.addGrassSquares(static_cast<int>(placedPositions.size()));

            int boardSize = board.getSize();
            std::set<Position> checkedBonuses;
            std::set<Position> coveredBonuses;
            
            // Marquer les bonus directement couverts par les tuiles (ils sont perdus)
            for (const auto& tilePos : placedPositions) {
                if (board.isBonusSquare(tilePos.row, tilePos.col)) {
                    coveredBonuses.insert(tilePos);
                }
            }
            
            // Vérifier les bonus autour des tuiles placées (4 directions cardinales)
            // Un bonus est capturé si les 4 cases autour sont remplies par le joueur
            for (const auto& tilePos : placedPositions) {
                int offsets[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                for (int i = 0; i < 4; ++i) {
                    int nr = tilePos.row + offsets[i][0];
                    int nc = tilePos.col + offsets[i][1];
                    if (nr >= 0 && nr < boardSize && nc >= 0 && nc < boardSize) {
                        Position bonusPos(nr, nc);
                        // Ignorer les bonus déjà couverts ou déjà vérifiés
                        if (coveredBonuses.find(bonusPos) != coveredBonuses.end()) {
                            continue;
                        }
                        if (checkedBonuses.find(bonusPos) == checkedBonuses.end() && 
                            board.isBonusSquare(nr, nc)) {
                            checkedBonuses.insert(bonusPos);
                            if (board.checkBonusCapture(nr, nc, player.getId())) {
                                board.claimBonusSquare(nr, nc, player.getId());
                                bonusManager.processBonusCapture(nr, nc, player.getId(), player, players);
                            }
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
            if (!validator.isWithinBounds(currentTile, candidate)) {
                std::cout << "Raison: La tuile depasse les limites du plateau." << std::endl;
            } else if (!validator.noOverlap(currentTile, candidate, player.getId())) {
                std::cout << "Raison: La tuile chevauche une autre tuile." << std::endl;
            } else if (!validator.noStoneConflict(currentTile, candidate)) {
                std::cout << "Raison: La tuile chevauche une pierre." << std::endl;
            } else if (!validator.noEnemyContact(currentTile, candidate, player.getId())) {
                std::cout << "Raison: La tuile touche le territoire d'un autre joueur." << std::endl;
            } else if (!validator.touchesTerritory(currentTile, candidate, player.getId()) && !isFirstTile) {
                std::cout << "Raison: La tuile ne touche pas votre territoire." << std::endl;
            } else if (isFirstTile) {
                std::cout << "Raison: La tuile ne touche pas votre tuile de depart." << std::endl;
            }
            if (!InputHandler::confirmAction("Voulez-vous reessayer ?")) {
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

    while (running && tileQueue.hasNext() && currentRound < maxRounds) {
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
                    Position pos = InputHandler::getTilePosition(board.getSize());
                    
                    bool isFirstTile = (player.getGrassSquaresOwned() == 1);
                    if (validator.isValidPlacement(smallTile, pos, player, isFirstTile)) {
                        board.placeTile(smallTile, pos, player.getId());
                        board.addTerritory(player.getId(), pos);
                        player.addGrassSquares(1);
                        if (player.useExchangeCoupon()) {
                            std::cout << "Tuile 1x1 placee." << std::endl;
                        }
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
        int maxSquares = 0;
        winners.clear();
        for (const auto& player : players) {
            if (player.getGrassSquaresOwned() > maxSquares) {
                maxSquares = player.getGrassSquaresOwned();
                winners.clear();
                winners.push_back(player.getId());
            } else if (player.getGrassSquaresOwned() == maxSquares) {
                winners.push_back(player.getId());
            }
        }
    }
    
    ConsoleUtils::clearConsole();
    std::cout << "=== RESULTATS FINAUX ===" << std::endl;
    std::cout << std::endl;
    
    for (const auto& player : players) {
        SquareResult result = Algorithms::findLargestSquare(board, player.getId());
        std::cout << ConsoleUtils::colorForPlayer(player.getId()) << player.getName() << ConsoleUtils::RESET_COLOR 
                  << " - Plus grand carre: " << result.size << "x" << result.size 
                  << " | Cases totales: " << player.getGrassSquaresOwned() << std::endl;
    }
    
    std::cout << std::endl;
    if (winners.size() == 1) {
        for (const auto& player : players) {
            if (player.getId() == winners[0]) {
                std::cout << ">>> GAGNANT: " << ConsoleUtils::colorForPlayer(player.getId()) 
                          << player.getName() << ConsoleUtils::RESET_COLOR << " <<<" << std::endl;
                break;
            }
        }
    } else {
        std::cout << ">>> EGALITE entre: ";
        for (size_t i = 0; i < winners.size(); ++i) {
            for (const auto& player : players) {
                if (player.getId() == winners[i]) {
                    std::cout << ConsoleUtils::colorForPlayer(player.getId()) 
                              << player.getName() << ConsoleUtils::RESET_COLOR;
                    if (i < winners.size() - 1) std::cout << ", ";
                    break;
                }
            }
        }
        std::cout << " <<<" << std::endl;
    }
}

#include "../include/Game.hpp"
#include "../include/GameUtils.hpp"
#include "../include/ConsoleUtils.hpp"
#include "../include/Constants.hpp"
#include <iostream>

void Game::showTurnIntro(const Player& player) const {
    std::cout << '\n';
    int roundNumber = currentRound + 1;
    int turnInRound = currentPlayerIndex + 1;
    std::cout << "Round " << roundNumber << "/" << TILES_PER_PLAYER 
              << " | Tour " << turnInRound << "/" << static_cast<int>(players.size())
              << " | Joueur " << player.getId() << " : " 
              << ConsoleUtils::colorForPlayer(player.getId()) << player.getName() << ConsoleUtils::RESET_COLOR << '\n';
}

void Game::showBoard() const {
    int size = board.getSize();
    std::cout << "   ";
    for (int col = 0; col < size && col < 26; ++col) {
        std::cout << static_cast<char>('A' + col);
    }
    if (size > 26) {
        for (int col = 26; col < size; ++col) {
            std::cout << static_cast<char>('A' + (col - 26));
        }
    }
    std::cout << '\n';

    for (int row = 0; row < size; ++row) {
        if (row < 26) {
            std::cout << static_cast<char>('A' + row) << "  ";
        } else {
            std::cout << static_cast<char>('A' + (row - 26)) << "  ";
        }
        for (int col = 0; col < size; ++col) {
            int cell = board.getCell(row, col);
            if (cell != 0) {
                std::cout << ConsoleUtils::colorForPlayer(cell) << '#' << ConsoleUtils::RESET_COLOR;
            } else if (board.hasStone(row, col)) {
                std::cout << 'S';
            } else if (board.isBonusSquare(row, col)) {
                BonusType type = board.getBonusType(row, col);
                if (type == BonusType::TILE_EXCHANGE) {
                    std::cout << 'E';
                } else if (type == BonusType::STONE) {
                    std::cout << 'P';
                } else if (type == BonusType::ROBBERY) {
                    std::cout << 'R';
                } else {
                    std::cout << 'B';
                }
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }
    std::cout << "Legende: # = tuile joueur, . = vide, S = pierre, E = echange, P = pierre bonus, R = vol\n";
}

void Game::showPlayers() const {
    std::cout << "Participants:" << '\n';
    for (const auto& player : players) {
        std::cout << ConsoleUtils::colorForPlayer(player.getId()) << "#" << player.getId() << " " << player.getName() << ConsoleUtils::RESET_COLOR 
                  << " | Tuiles: " << player.getGrassTilesPlaced() 
                  << " | Coupons: " << player.getExchangeCoupons() << '\n';
    }
}

void Game::showSummary() const {
    std::cout << '\n';
    std::cout << "Fin de partie apres " << currentRound + 1 << " tours complets." << '\n';
    showPlayers();
}

void Game::showTile(const Tile& tile) const {
    GameUtils::showTileConsole(tile);
}

void Game::showTiles(const std::vector<Tile>& tiles) const {
    GameUtils::showTilesConsole(tiles);
}


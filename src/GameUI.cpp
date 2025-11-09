#include "../include/Game.hpp"
#include "../include/GameUtils.hpp"
#include "../include/ConsoleUtils.hpp"
#include <iostream>

void Game::showTurnIntro(const Player& player) const {
    std::cout << '\n';
    std::cout << "Tour " << currentRound + 1 << " | Joueur " << player.getId() << " : " << ConsoleUtils::colorForPlayer(player.getId()) << player.getName() << ConsoleUtils::RESET_COLOR << '\n';
}

void Game::showBoard() const {
    int size = board.getSize();
    // header letters
    std::cout << "   ";
    for (int col = 0; col < size; ++col) {
        std::cout << static_cast<char>('A' + (col % 26));
    }
    std::cout << '\n';

    for (int row = 0; row < size; ++row) {
        // row label
        std::cout << static_cast<char>('A' + (row % 26)) << "  ";
        for (int col = 0; col < size; ++col) {
            int cell = board.getCell(row, col);
            if (cell == 0) {
                std::cout << '.';
            } else {
                std::cout << ConsoleUtils::colorForPlayer(cell) << '#' << ConsoleUtils::RESET_COLOR;
            }
        }
        std::cout << '\n';
    }
}

void Game::showPlayers() const {
    std::cout << "Participants:" << '\n';
    for (const auto& player : players) {
        std::cout << ConsoleUtils::colorForPlayer(player.getId()) << "#" << player.getId() << " " << player.getName() << ConsoleUtils::RESET_COLOR << " | Tuiles posees: " << player.getGrassTilesPlaced() << '\n';
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


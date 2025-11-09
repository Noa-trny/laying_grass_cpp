#include "../include/Display.hpp"
#include "../include/Queue.hpp"
#include <iostream>

void Display::renderBoard(const Board& board) {
    int size = board.getSize();
    std::cout << "Plateau " << size << "x" << size << "\n";
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            int cell = board.getCell(r, c);
            std::cout << (cell == 0 ? '.' : '#');
        }
        std::cout << '\n';
    }
}

void Display::renderPlayerInfo(const std::vector<Player>& players, int currentPlayerId) {
    for (const auto& p : players) {
        std::cout << (p.getId() == currentPlayerId ? "> " : "  ")
                  << "#" << p.getId() << " " << p.getName()
                  << " | Tuiles: " << p.getGrassTilesPlaced() << "\n";
    }
}

void Display::renderTile(const Tile& tile, int index) {
    std::cout << "[" << index << "] Tuile #" << tile.getId() << "\n";
    auto shape = tile.getShape();
    int h = static_cast<int>(shape.size());
    int w = 0; for (auto& row : shape) w = std::max(w, static_cast<int>(row.size()));
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            bool v = (r < (int)shape.size() && c < (int)shape[r].size()) ? shape[r][c] : false;
            std::cout << (v ? '#' : '.');
        }
        std::cout << '\n';
    }
}

void Display::renderTileOptions(const std::vector<Tile>& tiles) {
    for (size_t i = 0; i < tiles.size(); ++i) renderTile(tiles[i], static_cast<int>(i+1));
}

void Display::renderQueueStatus(const Queue& /*queue*/, int remaining) {
    std::cout << "Tuiles restantes: " << remaining << "\n";
}

void Display::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\x1B[2J\x1B[H";
#endif
}

void Display::showMessage(const std::string& message) { std::cout << message << "\n"; }
void Display::showPlacementOptions(const Tile& tile) {
    std::cout << "Placer la tuile #" << tile.getId() << " (saisissez ligne/colonne et orientation).\n";
}

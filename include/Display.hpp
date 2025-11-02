#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include <vector>

class Display {
public:
    static void renderBoard(const Board& board);
    static void renderPlayerInfo(const std::vector<Player>& players, int currentPlayerId);
    static void renderTile(const Tile& tile, int index);
    static void renderTileOptions(const std::vector<Tile>& tiles);
    static void renderQueueStatus(const Queue& queue, int remaining);
    static void clearScreen();
    static void showMessage(const std::string& message);
    static void showPlacementOptions(const Tile& tile);
};

#endif


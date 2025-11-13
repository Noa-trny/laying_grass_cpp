#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include "Tile.hpp"
#include "Position.hpp"
#include "Constants.hpp"
#include <string>
#include <vector>

class InputHandler {
public:
    static Position getTilePosition(int boardSize);
    static int getTileOrientation(const Tile& tile);
    static int getTileSelection(const std::vector<Tile>& options);
    static bool confirmAction(const std::string& prompt);
    static int getMenuChoice(const std::vector<std::string>& options);
    static Position getStonePosition(int boardSize);
    static int getRobberyTarget(const std::vector<int>& availablePlayers);
    static std::string getPlayerName(int playerId);
    static int getPlayerCount();
    static int chooseColor(const std::vector<std::string>& colorNames);
};

#endif

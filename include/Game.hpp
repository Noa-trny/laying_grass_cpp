#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "InputHandler.hpp"
#include "Tile.hpp"
#include <vector>
#include <string>

class Game {
private:
    Board board;
    std::vector<Player> players;
    int currentRound;
    int currentPlayerIndex;
    bool running;

    void initializePlayers(int numPlayers);
    void showTurnIntro(const Player& player) const;
    void handlePlayerTurn(Player& player);
    void advanceTurn();
    void showBoard() const;
    void showPlayers() const;
    void showSummary() const;

    // Tile management
    std::vector<Tile> availableTiles; // toutes les tuiles chargées depuis le JSON
    void loadTilesFromJson(const std::string& filepath);
    void loadTilesFromJson(); // essayer plusieurs chemins candidats
    std::vector<Tile> getRandomTiles(int n) const;
    void showTile(const Tile& tile) const;
    void showTiles(const std::vector<Tile>& tiles) const;

public:
    Game(int numPlayers);

    void run();
};

#endif

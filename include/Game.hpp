#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "InputHandler.hpp"
#include "Tile.hpp"
#include "Queue.hpp"
#include <vector>
#include <string>

class Game {
private:
    Board board;
    std::vector<Player> players;
    int currentRound;
    int currentPlayerIndex;
    bool running;

    Queue tileQueue; // file des tuiles
    Tile currentTile; // tuile en cours de placement
    bool hasCurrentTile;

    void initializePlayers(int numPlayers);
    void showTurnIntro(const Player& player) const;
    void handlePlayerTurn(Player& player);
    void advanceTurn();
    void showBoard() const;
    void showPlayers() const;
    void showSummary() const;

    // Chargement / gestion tuiles
    std::vector<Tile> availableTiles; // toutes les tuiles chargées depuis le JSON
    void loadTilesFromJson(const std::string& filepath);
    void loadTilesFromJson(); // essaie chemins candidats
    std::vector<Tile> getRandomTiles(int n) const;
    void showTile(const Tile& tile) const;
    void showTiles(const std::vector<Tile>& tiles) const;

    void drawNextTile();
    void rotateCurrentTile();

    void placeCurrentTile(Player& player);
    void exchangeCurrentTile(Player& player);

public:
    Game(int numPlayers);

    void run();
};

#endif

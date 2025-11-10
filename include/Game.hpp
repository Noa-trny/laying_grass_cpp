#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "InputHandler.hpp"
#include "Tile.hpp"
#include "Queue.hpp"
#include "Validator.hpp"
#include "Bonus.hpp"
#include "Algorithms.hpp"
#include <vector>
#include <string>

class Game {
private:
    Board board;
    std::vector<Player> players;
    Queue tileQueue;
    Validator validator;
    BonusManager bonusManager;
    int currentRound;
    int currentPlayerIndex;
    bool running;
    bool gameFinished;

    void initializePlayers(int numPlayers);
    void initializeStartingTiles();
    void showTurnIntro(const Player& player) const;
    void handlePlayerTurn(Player& player);
    void advanceTurn();
    void showBoard() const;
    void showPlayers() const;
    void showSummary() const;
    void showTile(const Tile& tile) const; // déclaration ajoutée
    void showTiles(const std::vector<Tile>& tiles) const; // déclaration ajoutée
    void processFinalPhase();
    void determineWinner() const;

public:
    Game(int numPlayers);

    void run();
};

#endif

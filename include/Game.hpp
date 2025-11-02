#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "Queue.hpp"
#include "Validator.hpp"
#include "BonusManager.hpp"
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
    
    void initializeGame(int numPlayers);
    void initializePlayers(int numPlayers);
    void placeStartingTiles();
    
    bool processPlayerTurn(Player& player);
    bool placeTile(Player& player);
    void handleBonusCapture(int row, int col, Player& player);
    
    void purchaseSmallTiles(Player& player);
    
    void endGame();
    Player findWinner();
    void displayResults();
    
public:
    Game(int numPlayers);
    
    void run();
    void pause();
    void resume();
};

#endif


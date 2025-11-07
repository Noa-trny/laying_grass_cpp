#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "InputHandler.hpp"
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
    
public:
    Game(int numPlayers);
    
    void run();
};

#endif

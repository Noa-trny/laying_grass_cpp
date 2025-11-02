#ifndef BONUS_HPP
#define BONUS_HPP

#include "Constants.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include <vector>

class BonusManager {
private:
    Board* board;
    
public:
    BonusManager(Board* board);
    
    bool activateTileExchange(Position pos, Player& player);
    bool activateStone(Position pos, Player& player);
    bool activateRobbery(Position pos, Player& fromPlayer, Player& toPlayer);
    
    void processBonusCapture(int row, int col, int playerId, Player& player);
};

#endif


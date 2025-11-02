#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include "Board.hpp"
#include "Tile.hpp"
#include "Player.hpp"

class Validator {
private:
    Board* board;
    
public:
    Validator(Board* board);
    
    bool isValidPlacement(const Tile& tile, Position pos, const Player& player, bool isFirstTile) const;
    bool isWithinBounds(const Tile& tile, Position pos) const;
    bool touchesTerritory(const Tile& tile, Position pos, int playerId) const;
    bool noOverlap(const Tile& tile, Position pos, int playerId) const;
    bool noEnemyContact(const Tile& tile, Position pos, int playerId) const;
    bool noStoneConflict(const Tile& tile, Position pos) const;
};

#endif


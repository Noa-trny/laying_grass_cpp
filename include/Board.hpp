#ifndef BOARD_HPP
#define BOARD_HPP

#include "Constants.hpp"
#include "Tile.hpp"
#include "Position.hpp"
#include <vector>
#include <set>
#include <map>

class Board {
private:
    int size;
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<bool>> bonusGrid;
    std::vector<std::vector<BonusType>> bonusTypes;
    std::vector<std::vector<bool>> stoneGrid;
    std::map<int, std::set<Position>> playerTerritories;
    bool isInside(int row, int col) const;

public:
    Board(int numPlayers);
    
    int getSize() const;
    int getCell(int row, int col) const;
    bool isBonusSquare(int row, int col) const;
    BonusType getBonusType(int row, int col) const;
    bool hasStone(int row, int col) const;
    bool isInside(Position pos) const;
    bool isOwnedBy(int row, int col, int playerId) const;
    
    void placeTile(const Tile& tile, Position pos, int playerId);
    void setCell(int row, int col, int playerId);
    bool canPlaceTile(const Tile& tile, Position pos, int playerId) const;
    
    void placeStone(Position pos);
    void removeStone(Position pos, bool usedCoupon);
    
    void addTerritory(int playerId, Position pos);
    void removeTerritory(int playerId, Position pos);
    std::set<Position> getTerritory(int playerId) const;
    
    bool isConnectedToTerritory(const Tile& tile, Position pos, int playerId) const;
    bool overlapsOtherPlayers(const Tile& tile, Position pos, int playerId) const;
    bool touchesOtherPlayers(const Tile& tile, Position pos, int playerId) const;
    
    void initializeBonusSquares(int numPlayers);
    
    bool checkBonusCapture(int row, int col, int playerId);
    void claimBonusSquare(int row, int col, int playerId);
};

#endif


#include "../include/Board.hpp"

Board::Board(int numPlayers)
    : size(numPlayers <= SMALL_GRID_THRESHOLD ? SMALL_GRID_SIZE : LARGE_GRID_SIZE),
      grid(size, std::vector<int>(size, 0)),
      bonusGrid(size, std::vector<bool>(size, false)),
      bonusTypes(size, std::vector<BonusType>(size, BonusType::TILE_EXCHANGE)),
      stoneGrid(size, std::vector<bool>(size, false)) {}

int Board::getSize() const { return size; }
int Board::getCell(int row, int col) const { return (row>=0 && row<size && col>=0 && col<size) ? grid[row][col] : 0; }
bool Board::isBonusSquare(int row, int col) const { return (row>=0 && row<size && col>=0 && col<size) ? bonusGrid[row][col] : false; }
BonusType Board::getBonusType(int row, int col) const { return bonusTypes[row][col]; }
bool Board::hasStone(int row, int col) const { return (row>=0 && row<size && col>=0 && col<size) ? stoneGrid[row][col] : false; }

void Board::placeTile(const Tile&, Position, int) {}
bool Board::canPlaceTile(const Tile&, Position, int) const { return true; }

void Board::placeStone(Position) {}
void Board::removeStone(Position, bool) {}

void Board::addTerritory(int playerId, Position pos) { playerTerritories[playerId].insert(pos); }
std::set<Position> Board::getTerritory(int playerId) const {
    auto it = playerTerritories.find(playerId);
    if (it != playerTerritories.end()) return it->second;
    return {};
}

bool Board::isConnectedToTerritory(const Tile&, Position, int) const { return true; }
bool Board::overlapsOtherPlayers(const Tile&, Position, int) const { return false; }
bool Board::touchesOtherPlayers(const Tile&, Position, int) const { return false; }

void Board::initializeBonusSquares(int) {}

bool Board::checkBonusCapture(int, int, int) { return false; }


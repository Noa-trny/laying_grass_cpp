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

void Board::placeTile(const Tile& tile, Position pos, int playerId) {
    auto shape = tile.getShape();
    int h = static_cast<int>(shape.size());
    int w = h ? static_cast<int>(shape[0].size()) : 0;
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            if (!shape[r][c]) continue;
            int rr = pos.row + r;
            int cc = pos.col + c;
            if (rr >= 0 && rr < size && cc >= 0 && cc < size) {
                grid[rr][cc] = playerId;
            }
        }
    }
}

bool Board::canPlaceTile(const Tile& tile, Position pos, int playerId) const {
    auto shape = tile.getShape();
    int h = static_cast<int>(shape.size());
    if (h == 0) return false;
    int w = static_cast<int>(shape[0].size());
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            if (!shape[r][c]) continue;
            int rr = pos.row + r;
            int cc = pos.col + c;
            // en dehors du plateau
            if (rr < 0 || rr >= size || cc < 0 || cc >= size) return false;
            // cellule occupée par une tuile
            if (grid[rr][cc] != 0) return false;
            // pierre présente
            if (stoneGrid[rr][cc]) return false;
        }
    }
    // Pour l'instant pas de vérification de connexion/territoire
    return true;
}

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

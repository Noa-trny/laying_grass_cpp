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
    if (!canPlaceTile(tile, pos, playerId)) return;
    auto shape = tile.getShape();
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (!shape[r][c]) continue;
            int br = pos.row + r;
            int bc = pos.col + c;
            if (br >= 0 && br < size && bc >= 0 && bc < size) {
                grid[br][bc] = playerId;
                addTerritory(playerId, Position{br, bc});
            }
        }
    }
}

bool Board::canPlaceTile(const Tile& tile, Position pos, int playerId) const {
    auto shape = tile.getShape();
    if (shape.empty()) return false;
    // verifie sortie des limites ou collision pierre / autre joueur
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (!shape[r][c]) continue;
            int br = pos.row + r;
            int bc = pos.col + c;
            if (br < 0 || br >= size || bc < 0 || bc >= size) return false;
            int occupant = grid[br][bc];
            if (occupant != 0 && occupant != playerId) return false;
            if (hasStone(br, bc)) return false;
        }
    }
    // simple regle de connexite: la nouvelle tuile doit toucher au moins une case deja occupee du joueur si le joueur possede un territoire
    auto terr = getTerritory(playerId);
    if (!terr.empty()) {
        bool touches = false;
        for (int r = 0; r < static_cast<int>(shape.size()) && !touches; ++r) {
            for (int c = 0; c < static_cast<int>(shape[r].size()) && !touches; ++c) {
                if (!shape[r][c]) continue;
                int br = pos.row + r;
                int bc = pos.col + c;
                const Position p{br, bc};
                const Position up{br-1, bc};
                const Position down{br+1, bc};
                const Position left{br, bc-1};
                const Position right{br, bc+1};
                if (terr.count(up) || terr.count(down) || terr.count(left) || terr.count(right)) {
                    touches = true;
                }
            }
        }
        if (!touches) return false;
    }
    return true;
}

void Board::placeStone(Position pos) {
    if (pos.row>=0 && pos.row<size && pos.col>=0 && pos.col<size) stoneGrid[pos.row][pos.col] = true;
}
void Board::removeStone(Position pos, bool) {
    if (pos.row>=0 && pos.row<size && pos.col>=0 && pos.col<size) stoneGrid[pos.row][pos.col] = false;
}

void Board::addTerritory(int playerId, Position pos) { playerTerritories[playerId].insert(pos); }
std::set<Position> Board::getTerritory(int playerId) const {
    auto it = playerTerritories.find(playerId);
    if (it != playerTerritories.end()) return it->second;
    return {};
}

bool Board::isConnectedToTerritory(const Tile& tile, Position pos, int playerId) const { return canPlaceTile(tile, pos, playerId); }
bool Board::overlapsOtherPlayers(const Tile&, Position, int) const { return false; }
bool Board::touchesOtherPlayers(const Tile&, Position, int) const { return false; }

void Board::initializeBonusSquares(int) {}

bool Board::checkBonusCapture(int, int, int) { return false; }

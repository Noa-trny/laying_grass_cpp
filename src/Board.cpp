#include "../include/Board.hpp"
#include <random>
#include <cmath>

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

void Board::setCell(int row, int col, int playerId) {
    if (row >= 0 && row < size && col >= 0 && col < size) {
        grid[row][col] = playerId;
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

void Board::placeStone(Position pos) {
    if (pos.row >= 0 && pos.row < size && pos.col >= 0 && pos.col < size) {
        stoneGrid[pos.row][pos.col] = true;
    }
}

void Board::removeStone(Position pos, bool usedCoupon) {
    if (usedCoupon && pos.row >= 0 && pos.row < size && pos.col >= 0 && pos.col < size) {
        stoneGrid[pos.row][pos.col] = false;
    }
}

void Board::addTerritory(int playerId, Position pos) { playerTerritories[playerId].insert(pos); }

void Board::removeTerritory(int playerId, Position pos) {
    auto it = playerTerritories.find(playerId);
    if (it != playerTerritories.end()) {
        it->second.erase(pos);
    }
}

std::set<Position> Board::getTerritory(int playerId) const {
    auto it = playerTerritories.find(playerId);
    if (it != playerTerritories.end()) return it->second;
    return {};
}

bool Board::isConnectedToTerritory(const Tile& tile, Position pos, int playerId) const {
    auto territory = getTerritory(playerId);
    if (territory.empty()) return false;
    
    auto shape = tile.getShape();
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (shape[r][c]) {
                Position tilePos(pos.row + r, pos.col + c);
                for (const auto& terrPos : territory) {
                    int dist = std::abs(tilePos.row - terrPos.row) + std::abs(tilePos.col - terrPos.col);
                    if (dist == 1) return true;
                }
            }
        }
    }
    return false;
}

bool Board::overlapsOtherPlayers(const Tile& tile, Position pos, int playerId) const {
    auto shape = tile.getShape();
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (shape[r][c]) {
                int rr = pos.row + r;
                int cc = pos.col + c;
                if (rr >= 0 && rr < size && cc >= 0 && cc < size) {
                    int cell = grid[rr][cc];
                    if (cell != 0 && cell != playerId) return true;
                }
            }
        }
    }
    return false;
}

bool Board::touchesOtherPlayers(const Tile& tile, Position pos, int playerId) const {
    auto shape = tile.getShape();
    int offsets[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (shape[r][c]) {
                int rr = pos.row + r;
                int cc = pos.col + c;
                for (int i = 0; i < 4; ++i) {
                    int nr = rr + offsets[i][0];
                    int nc = cc + offsets[i][1];
                    if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
                        int cell = grid[nr][nc];
                        if (cell != 0 && cell != playerId) return true;
                    }
                }
            }
        }
    }
    return false;
}

void Board::initializeBonusSquares(int numPlayers) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, size - 2);
    
    int tileExchangeCount = static_cast<int>(std::ceil(1.5 * numPlayers));
    int stoneCount = static_cast<int>(std::ceil(0.5 * numPlayers));
    int robberyCount = numPlayers;
    
    int placed = 0;
    int attempts = 0;
    
    while (placed < tileExchangeCount && attempts < 1000) {
        int row = dis(gen);
        int col = dis(gen);
        
        if (!bonusGrid[row][col] && grid[row][col] == 0) {
            bool tooClose = false;
            for (int r = row - 1; r <= row + 1; ++r) {
                for (int c = col - 1; c <= col + 1; ++c) {
                    if (r >= 0 && r < size && c >= 0 && c < size && bonusGrid[r][c]) {
                        tooClose = true;
                        break;
                    }
                }
                if (tooClose) break;
            }
            
            if (!tooClose && row > 0 && row < size - 1 && col > 0 && col < size - 1) {
                bonusGrid[row][col] = true;
                bonusTypes[row][col] = BonusType::TILE_EXCHANGE;
                placed++;
            }
        }
        attempts++;
    }
    
    placed = 0;
    attempts = 0;
    while (placed < stoneCount && attempts < 1000) {
        int row = dis(gen);
        int col = dis(gen);
        
        if (!bonusGrid[row][col] && grid[row][col] == 0) {
            bool tooClose = false;
            for (int r = row - 1; r <= row + 1; ++r) {
                for (int c = col - 1; c <= col + 1; ++c) {
                    if (r >= 0 && r < size && c >= 0 && c < size && bonusGrid[r][c]) {
                        tooClose = true;
                        break;
                    }
                }
                if (tooClose) break;
            }
            
            if (!tooClose && row > 0 && row < size - 1 && col > 0 && col < size - 1) {
                bonusGrid[row][col] = true;
                bonusTypes[row][col] = BonusType::STONE;
                placed++;
            }
        }
        attempts++;
    }
    
    placed = 0;
    attempts = 0;
    while (placed < robberyCount && attempts < 1000) {
        int row = dis(gen);
        int col = dis(gen);
        
        if (!bonusGrid[row][col] && grid[row][col] == 0) {
            bool tooClose = false;
            for (int r = row - 1; r <= row + 1; ++r) {
                for (int c = col - 1; c <= col + 1; ++c) {
                    if (r >= 0 && r < size && c >= 0 && c < size && bonusGrid[r][c]) {
                        tooClose = true;
                        break;
                    }
                }
                if (tooClose) break;
            }
            
            if (!tooClose && row > 0 && row < size - 1 && col > 0 && col < size - 1) {
                bonusGrid[row][col] = true;
                bonusTypes[row][col] = BonusType::ROBBERY;
                placed++;
            }
        }
        attempts++;
    }
}

bool Board::checkBonusCapture(int row, int col, int playerId) {
    if (!isBonusSquare(row, col)) return false;
    
    int offsets[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    bool allSurrounded = true;
    
    for (int i = 0; i < 4; ++i) {
        int nr = row + offsets[i][0];
        int nc = col + offsets[i][1];
        if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
            if (grid[nr][nc] != playerId) {
                allSurrounded = false;
                break;
            }
        } else {
            allSurrounded = false;
            break;
        }
    }
    
    return allSurrounded;
}

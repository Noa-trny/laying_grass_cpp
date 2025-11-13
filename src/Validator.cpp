#include "../include/Validator.hpp"
#include "../include/Algorithms.hpp"
#include <algorithm>
#include <cmath>

Validator::Validator(Board* board_) : board(board_) {}

bool Validator::isValidPlacement(const Tile& tile, Position pos, const Player& player, bool isFirstTile) const {
    if (!isWithinBounds(tile, pos)) return false;
    if (!noOverlap(tile, pos, player.getId())) return false;
    if (!noStoneConflict(tile, pos)) return false;
    if (!noEnemyContact(tile, pos, player.getId())) return false;
    
    if (isFirstTile) {
        Position startPos = player.getStartPosition();
        auto shape = tile.getShape();
        bool touchesStart = false;
        for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
            for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
                if (shape[r][c]) {
                    Position tilePos(pos.row + r, pos.col + c);
                    int dist = std::abs(tilePos.row - startPos.row) + std::abs(tilePos.col - startPos.col);
                    if (dist == 1) {
                        touchesStart = true;
                        break;
                    }
                }
            }
            if (touchesStart) break;
        }
        return touchesStart;
    } else {
        return touchesTerritory(tile, pos, player.getId());
    }
}

bool Validator::isWithinBounds(const Tile& tile, Position pos) const {
    auto shape = tile.getShape();
    int size = board->getSize();
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (shape[r][c]) {
                int rr = pos.row + r;
                int cc = pos.col + c;
                if (rr < 0 || rr >= size || cc < 0 || cc >= size) return false;
            }
        }
    }
    return true;
}

bool Validator::touchesTerritory(const Tile& tile, Position pos, int playerId) const {
    auto territory = board->getTerritory(playerId);
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

bool Validator::noOverlap(const Tile& tile, Position pos, int playerId) const {
    auto shape = tile.getShape();
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (shape[r][c]) {
                int rr = pos.row + r;
                int cc = pos.col + c;
                int cell = board->getCell(rr, cc);
                if (cell != 0) return false;
            }
        }
    }
    return true;
}

bool Validator::noEnemyContact(const Tile& tile, Position pos, int playerId) const {
    auto shape = tile.getShape();
    int size = board->getSize();
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (shape[r][c]) {
                int rr = pos.row + r;
                int cc = pos.col + c;
                
                int offsets[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                for (int i = 0; i < 4; ++i) {
                    int nr = rr + offsets[i][0];
                    int nc = cc + offsets[i][1];
                    if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
                        int cell = board->getCell(nr, nc);
                        if (cell != 0 && cell != playerId) return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Validator::noStoneConflict(const Tile& tile, Position pos) const {
    auto shape = tile.getShape();
    for (int r = 0; r < static_cast<int>(shape.size()); ++r) {
        for (int c = 0; c < static_cast<int>(shape[r].size()); ++c) {
            if (shape[r][c]) {
                int rr = pos.row + r;
                int cc = pos.col + c;
                if (board->hasStone(rr, cc)) return false;
            }
        }
    }
    return true;
}


#include "../include/Algorithms.hpp"
#include <algorithm>
#include <queue>

SquareResult Algorithms::findLargestSquare(const Board& board, int playerId) {
    SquareResult result;
    result.size = 0;
    result.topLeft = Position(-1, -1);
    
    int size = board.getSize();
    std::vector<std::vector<int>> dp(size, std::vector<int>(size, 0));
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board.getCell(i, j) == playerId) {
                if (i == 0 || j == 0) {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] = std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
                }
                
                if (dp[i][j] > result.size) {
                    result.size = dp[i][j];
                    result.topLeft = Position(i - result.size + 1, j - result.size + 1);
                }
            } else {
                dp[i][j] = 0;
            }
        }
    }
    
    if (result.size > 0) {
        for (int i = result.topLeft.row; i < result.topLeft.row + result.size; ++i) {
            for (int j = result.topLeft.col; j < result.topLeft.col + result.size; ++j) {
                result.positions.insert(Position(i, j));
            }
        }
    }
    
    return result;
}

bool Algorithms::isConnectedComponent(const Board& board, int playerId, const std::set<Position>& territory) {
    if (territory.empty()) return true;
    
    std::queue<Position> q;
    std::set<Position> visited;
    
    Position start = *territory.begin();
    q.push(start);
    visited.insert(start);
    
    int offsets[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    
    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        
        for (int i = 0; i < 4; ++i) {
            Position next(current.row + offsets[i][0], current.col + offsets[i][1]);
            if (territory.find(next) != territory.end() && visited.find(next) == visited.end()) {
                visited.insert(next);
                q.push(next);
            }
        }
    }
    
    return visited.size() == territory.size();
}

std::set<Position> Algorithms::getConnectedTerritory(const Board& board, int playerId, Position start) {
    std::set<Position> result;
    std::queue<Position> q;
    std::set<Position> visited;
    
    if (board.getCell(start.row, start.col) != playerId) return result;
    
    q.push(start);
    visited.insert(start);
    result.insert(start);
    
    int offsets[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int size = board.getSize();
    
    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        
        for (int i = 0; i < 4; ++i) {
            Position next(current.row + offsets[i][0], current.col + offsets[i][1]);
            if (next.row >= 0 && next.row < size && next.col >= 0 && next.col < size) {
                if (board.getCell(next.row, next.col) == playerId && visited.find(next) == visited.end()) {
                    visited.insert(next);
                    result.insert(next);
                    q.push(next);
                }
            }
        }
    }
    
    return result;
}

std::vector<Position> Algorithms::getAdjacentPositions(Position pos, int boardSize) {
    std::vector<Position> result;
    int offsets[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    
    for (int i = 0; i < 4; ++i) {
        Position next(pos.row + offsets[i][0], pos.col + offsets[i][1]);
        if (next.row >= 0 && next.row < boardSize && next.col >= 0 && next.col < boardSize) {
            result.push_back(next);
        }
    }
    
    return result;
}


#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Board.hpp"
#include "Position.hpp"
#include <vector>
#include <set>

struct SquareResult {
    int size;
    Position topLeft;
    std::set<Position> positions;
};

class Algorithms {
public:
    static SquareResult findLargestSquare(const Board& board, int playerId);
    static bool isConnectedComponent(const Board& board, int playerId, const std::set<Position>& territory);
    static std::set<Position> getConnectedTerritory(const Board& board, int playerId, Position start);
    static std::vector<Position> getAdjacentPositions(Position pos, int boardSize);
};

#endif


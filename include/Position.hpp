#ifndef POSITION_HPP
#define POSITION_HPP

struct Position {
    int row;
    int col;
    
    Position() : row(0), col(0) {}
    Position(int r, int c) : row(r), col(c) {}
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool operator<(const Position& other) const {
        if (row != other.row) return row < other.row;
        return col < other.col;
    }
};

#endif


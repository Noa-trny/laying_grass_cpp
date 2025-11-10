#ifndef TILE_HPP
#define TILE_HPP

#include <vector>
#include "Position.hpp"

class Tile {
private:
    int id;
    std::vector<std::vector<bool>> shape;
    int width;
    int height;
    
public:
    Tile(); // Default constructor added
    Tile(int id, const std::vector<std::vector<bool>>& shape);
    
    int getId() const;
    int getWidth() const;
    int getHeight() const;
    std::vector<std::vector<bool>> getShape() const;
    
    Tile rotate90() const;
    Tile rotate180() const;
    Tile rotate270() const;
    Tile flipHorizontal() const;
    Tile flipVertical() const;
    
    std::vector<Tile> getAllOrientations() const;
    
    bool isValid() const;
};

#endif

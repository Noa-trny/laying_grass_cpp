#include "../include/Tile.hpp"

Tile::Tile(int id_, const std::vector<std::vector<bool>>& shape_)
    : id(id_), shape(shape_), width(shape_.empty() ? 0 : static_cast<int>(shape_[0].size())), height(static_cast<int>(shape_.size())) {}

int Tile::getId() const { return id; }
int Tile::getWidth() const { return width; }
int Tile::getHeight() const { return height; }
std::vector<std::vector<bool>> Tile::getShape() const { return shape; }

Tile Tile::rotate90() const { return *this; }
Tile Tile::rotate180() const { return *this; }
Tile Tile::rotate270() const { return *this; }
Tile Tile::flipHorizontal() const { return *this; }
Tile Tile::flipVertical() const { return *this; }

std::vector<Tile> Tile::getAllOrientations() const { return { *this }; }

bool Tile::isValid() const { return !shape.empty(); }


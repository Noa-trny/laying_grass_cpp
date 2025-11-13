#include "../include/Tile.hpp"

Tile::Tile()
    : id(0), shape(), width(0), height(0) {}

Tile::Tile(int id_, const std::vector<std::vector<bool>>& shape_)
    : id(id_), shape(shape_), width(shape_.empty() ? 0 : static_cast<int>(shape_[0].size())), height(static_cast<int>(shape_.size())) {}

int Tile::getId() const { return id; }
int Tile::getWidth() const { return width; }
int Tile::getHeight() const { return height; }
std::vector<std::vector<bool>> Tile::getShape() const { return shape; }

Tile Tile::rotate90() const {
    if (shape.empty()) return *this;
    int h = static_cast<int>(shape.size());
    int w = static_cast<int>(shape[0].size());
    std::vector<std::vector<bool>> s(w, std::vector<bool>(h, false));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            s[j][h - 1 - i] = shape[i][j];
        }
    }
    return Tile{id, s};
}

Tile Tile::rotate180() const {
    if (shape.empty()) return *this;
    int h = static_cast<int>(shape.size());
    int w = static_cast<int>(shape[0].size());
    std::vector<std::vector<bool>> s(h, std::vector<bool>(w, false));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            s[h - 1 - i][w - 1 - j] = shape[i][j];
        }
    }
    return Tile{id, s};
}

Tile Tile::rotate270() const {
    if (shape.empty()) return *this;
    // La rotation de 270° équivaut à trois rotations de 90°, mais est implémentée directement ici.
    int h = static_cast<int>(shape.size());
    int w = static_cast<int>(shape[0].size());
    std::vector<std::vector<bool>> s(w, std::vector<bool>(h, false));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            s[w - 1 - j][i] = shape[i][j];
        }
    }
    return Tile{id, s};
}

Tile Tile::flipHorizontal() const {
    if (shape.empty()) return *this;
    int h = static_cast<int>(shape.size());
    int w = static_cast<int>(shape[0].size());
    std::vector<std::vector<bool>> s(h, std::vector<bool>(w, false));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            s[i][w - 1 - j] = shape[i][j];
        }
    }
    return Tile{id, s};
}

Tile Tile::flipVertical() const {
    if (shape.empty()) return *this;
    int h = static_cast<int>(shape.size());
    int w = static_cast<int>(shape[0].size());
    std::vector<std::vector<bool>> s(h, std::vector<bool>(w, false));
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            s[h - 1 - i][j] = shape[i][j];
        }
    }
    return Tile{id, s};
}

std::vector<Tile> Tile::getAllOrientations() const {
    std::vector<Tile> out;
    out.push_back(*this);
    out.push_back(this->rotate90());
    out.push_back(this->rotate180());
    out.push_back(this->rotate270());
    return out;
}

bool Tile::isValid() const { return !shape.empty(); }

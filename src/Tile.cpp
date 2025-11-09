#include "../include/Tile.hpp"
#include <algorithm>
#include <set>

Tile::Tile(int id_, const std::vector<std::vector<bool>>& shape_)
    : id(id_), shape(shape_), width(shape_.empty() ? 0 : static_cast<int>(shape_[0].size())), height(static_cast<int>(shape_.size())) {}

int Tile::getId() const { return id; }
int Tile::getWidth() const { return width; }
int Tile::getHeight() const { return height; }
std::vector<std::vector<bool>> Tile::getShape() const { return shape; }

// Rotation horaire 90 deg
Tile Tile::rotate90() const {
    if (shape.empty()) return *this;
    int h = height;
    int w = width;
    std::vector<std::vector<bool>> rotated(w, std::vector<bool>(h, false));
    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            rotated[c][h - 1 - r] = shape[r][c];
        }
    }
    return Tile(id, rotated);
}

Tile Tile::rotate180() const { return rotate90().rotate90(); }
Tile Tile::rotate270() const { return rotate90().rotate90().rotate90(); }

Tile Tile::flipHorizontal() const {
    if (shape.empty()) return *this;
    std::vector<std::vector<bool>> flipped = shape;
    for (auto & row : flipped) {
        std::reverse(row.begin(), row.end());
    }
    return Tile(id, flipped);
}

Tile Tile::flipVertical() const {
    if (shape.empty()) return *this;
    std::vector<std::vector<bool>> flipped = shape;
    std::reverse(flipped.begin(), flipped.end());
    return Tile(id, flipped);
}

static bool shapesEqual(const std::vector<std::vector<bool>>& a, const std::vector<std::vector<bool>>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].size() != b[i].size()) return false;
        for (size_t j = 0; j < a[i].size(); ++j) {
            if (a[i][j] != b[i][j]) return false;
        }
    }
    return true;
}

std::vector<Tile> Tile::getAllOrientations() const {
    // Generer rotations et flips puis dedupliquer
    std::vector<Tile> variants;
    variants.reserve(8);
    Tile r0 = *this;
    Tile r90 = rotate90();
    Tile r180 = rotate180();
    Tile r270 = rotate270();
    Tile fH = flipHorizontal();
    // Flips des rotations pour couvrir miroirs potentiels
    Tile fH90 = r90.flipHorizontal();
    Tile fH180 = r180.flipHorizontal();
    Tile fH270 = r270.flipHorizontal();

    std::vector<Tile> all = {r0, r90, r180, r270, fH, fH90, fH180, fH270};
    for (const auto& t : all) {
        bool exists = false;
        for (const auto& kept : variants) {
            if (shapesEqual(kept.getShape(), t.getShape())) { exists = true; break; }
        }
        if (!exists) variants.push_back(t);
    }
    return variants;
}

bool Tile::isValid() const { return !shape.empty() && width > 0 && height > 0; }

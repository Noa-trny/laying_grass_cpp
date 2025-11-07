#ifndef TILEPARSER_HPP
#define TILEPARSER_HPP

#include "Tile.hpp"
#include <string>
#include <vector>

namespace TileParser {
    // Charge les tuiles depuis le fichier JSON simple et renvoie un vecteur de Tile
    // Lance std::runtime_error en cas d'erreur d'ouverture ou d'absence de tuiles
    std::vector<Tile> loadTilesFromJson(const std::string& filepath);
}

#endif // TILEPARSER_HPP


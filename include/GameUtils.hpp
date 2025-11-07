#ifndef GAMEUTILS_HPP
#define GAMEUTILS_HPP

#include "Tile.hpp"
#include <vector>

namespace GameUtils {
    // Retourne jusqu'à n tuiles aléatoires depuis la liste fournie
    std::vector<Tile> pickRandomTiles(const std::vector<Tile>& available, int n);

    // Affichage console d'une tuile
    void showTileConsole(const Tile& tile);

    // Affichage console de plusieurs tuiles côte-à-côte
    void showTilesConsole(const std::vector<Tile>& tiles);
}

#endif // GAMEUTILS_HPP


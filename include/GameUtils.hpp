#ifndef GAMEUTILS_HPP
#define GAMEUTILS_HPP

#include "Tile.hpp"
#include <vector>
#include <string>
#include "Board.hpp"
#include "Position.hpp"

namespace GameUtils {
    // Retourne jusqu'à n tuiles aléatoires depuis la liste fournie
    std::vector<Tile> pickRandomTiles(const std::vector<Tile>& available, int n);

    // Affichage console d'une tuile
    void showTileConsole(const Tile& tile);
    // Affichage console d'une tuile avec couleur ANSI
    // displayNumber : si >=0, utilise ce numéro comme label (ex. "(1)") au lieu de l'id
    void showTileConsoleColored(const Tile& tile, const std::string& color, const std::string& reset = "\x1b[0m", int displayNumber = -1);

    // Affichage console de plusieurs tuiles côte-à-côte
    void showTilesConsole(const std::vector<Tile>& tiles);
    // Affichage console de plusieurs tuiles côte-à-côte avec couleur
    // displayNumbers : si non vide, doit avoir une entrée par tuile et affichera ces nombres
    void showTilesConsoleColored(const std::vector<Tile>& tiles, const std::string& color, const std::string& reset = "\x1b[0m", const std::vector<int>& displayNumbers = {});

    // Affiche le plateau avec une tuile superposée en position pos (aperçu)
    void showBoardWithOverlay(const Board& board, const Tile& tile, Position pos, const std::string& overlayColor, const std::string& reset = "\x1b[0m");

}

#endif // GAMEUTILS_HPP

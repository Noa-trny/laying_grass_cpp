#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

constexpr int MIN_PLAYERS = 2;
constexpr int MAX_PLAYERS = 9;
constexpr int SMALL_GRID_SIZE = 20;
constexpr int LARGE_GRID_SIZE = 30;
constexpr int TILES_PER_PLAYER = 9;
constexpr int TOTAL_TILE_TYPES = 96;
constexpr int EXCHANGE_PREVIEW_SIZE = 5;

constexpr int SMALL_GRID_THRESHOLD = 4;

enum class BonusType {
    TILE_EXCHANGE,
    STONE,
    ROBBERY
};

enum class TileShape {
    GRASS,
    PATH
};

#endif


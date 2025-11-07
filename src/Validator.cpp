#include "../include/Validator.hpp"

Validator::Validator(Board* board_) : board(board_) {}

bool Validator::isValidPlacement(const Tile&, Position, const Player&, bool) const { return true; }
bool Validator::isWithinBounds(const Tile&, Position) const { return true; }
bool Validator::touchesTerritory(const Tile&, Position, int) const { return true; }
bool Validator::noOverlap(const Tile&, Position, int) const { return true; }
bool Validator::noEnemyContact(const Tile&, Position, int) const { return true; }
bool Validator::noStoneConflict(const Tile&, Position) const { return true; }


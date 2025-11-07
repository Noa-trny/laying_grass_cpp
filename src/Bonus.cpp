#include "../include/Bonus.hpp"

BonusManager::BonusManager(Board* board_) : board(board_) {}

bool BonusManager::activateTileExchange(Position, Player&) { return false; }
bool BonusManager::activateStone(Position, Player&) { return false; }
bool BonusManager::activateRobbery(Position, Player&, Player&) { return false; }

void BonusManager::processBonusCapture(int, int, int, Player&) {}


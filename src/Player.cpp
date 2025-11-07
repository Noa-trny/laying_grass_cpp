#include "../include/Player.hpp"

Player::Player(int id_, const std::string& name_, Position startPos_)
    : id(id_), name(name_), startPosition(startPos_), exchangeCoupons(0), grassTilesPlaced(0) {}

int Player::getId() const { return id; }
std::string Player::getName() const { return name; }
Position Player::getStartPosition() const { return startPosition; }
int Player::getExchangeCoupons() const { return exchangeCoupons; }
int Player::getGrassTilesPlaced() const { return grassTilesPlaced; }

void Player::addExchangeCoupon() { ++exchangeCoupons; }
bool Player::useExchangeCoupon() {
    if (exchangeCoupons > 0) { --exchangeCoupons; return true; }
    return false;
}

void Player::incrementGrassTiles() { ++grassTilesPlaced; }

bool Player::hasStartingTile() const { return true; }


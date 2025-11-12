#include "../include/Player.hpp"
#include <algorithm>

Player::Player(int id_, const std::string& name_, Position startPos_)
    : id(id_),
      name(name_),
      startPosition(startPos_),
      exchangeCoupons(0),
      grassSquaresOwned(0),
      colorName(""),
      colorCode("") {}

int Player::getId() const { return id; }
std::string Player::getName() const { return name; }
Position Player::getStartPosition() const { return startPosition; }
int Player::getExchangeCoupons() const { return exchangeCoupons; }
int Player::getGrassSquaresOwned() const { return grassSquaresOwned; }
std::string Player::getColorName() const { return colorName; }
std::string Player::getColorCode() const { return colorCode; }

void Player::addExchangeCoupon() { ++exchangeCoupons; }
bool Player::useExchangeCoupon() {
    if (exchangeCoupons > 0) { --exchangeCoupons; return true; }
    return false;
}

void Player::addGrassSquares(int amount) {
    if (amount > 0) {
        grassSquaresOwned += amount;
    }
}

void Player::removeGrassSquares(int amount) {
    if (amount > 0) {
        grassSquaresOwned = std::max(0, grassSquaresOwned - amount);
    }
}

void Player::setStartPosition(Position pos) { startPosition = pos; }

void Player::setColor(const std::string& name_, const std::string& code_) {
    colorName = name_;
    colorCode = code_;
}

bool Player::hasStartingTile() const { return grassSquaresOwned > 0; }


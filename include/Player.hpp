#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Position.hpp"
#include <vector>
#include <string>

class Player {
private:
    int id;
    std::string name;
    Position startPosition;
    int exchangeCoupons;
    int grassTilesPlaced;
    
public:
    Player(int id, const std::string& name, Position startPos);
    
    int getId() const;
    std::string getName() const;
    Position getStartPosition() const;
    int getExchangeCoupons() const;
    int getGrassTilesPlaced() const;
    
    void addExchangeCoupon();
    bool useExchangeCoupon();
    
    void incrementGrassTiles();
    void setStartPosition(Position pos);
    
    bool hasStartingTile() const;
};

#endif


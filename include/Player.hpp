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
    int grassSquaresOwned;
    std::string colorName;
    std::string colorCode;
    
public:
    Player(int id, const std::string& name, Position startPos);
    
    int getId() const;
    std::string getName() const;
    Position getStartPosition() const;
    int getExchangeCoupons() const;
    int getGrassSquaresOwned() const;
    std::string getColorName() const;
    std::string getColorCode() const;
    
    void addExchangeCoupon();
    bool useExchangeCoupon();
    
    void addGrassSquares(int amount);
    void removeGrassSquares(int amount);
    void setStartPosition(Position pos);
    void setColor(const std::string& name, const std::string& code);
    
    bool hasStartingTile() const;
};

#endif


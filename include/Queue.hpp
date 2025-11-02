#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Tile.hpp"
#include <vector>
#include <queue>

class Queue {
private:
    std::vector<Tile> mainQueue;
    std::vector<Tile> exchangeQueue;
    size_t currentIndex;
    int totalTiles;
    int numPlayers;
    
    void initializeQueue();
    void shuffleQueue();
    
public:
    Queue(int numPlayers);
    
    bool hasNext() const;
    Tile getNext();
    std::vector<Tile> peekNext(int count);
    
    Tile exchangeTile(int selectedIndex);
    
    void returnToQueue(const Tile& tile);
    
    int getRemainingCount() const;
};

#endif


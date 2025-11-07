#include "../include/Queue.hpp"
#include "../include/Constants.hpp"

Queue::Queue(int numPlayers_)
    : currentIndex(0), totalTiles(TILES_PER_PLAYER * numPlayers_), numPlayers(numPlayers_) {
    initializeQueue();
}

void Queue::initializeQueue() {
    mainQueue.clear();
    exchangeQueue.clear();
    currentIndex = 0;
    // Stub: remplir avec quelques tuiles minimales
    mainQueue.emplace_back(0, std::vector<std::vector<bool>>{{true}});
}

void Queue::shuffleQueue() {
    // Stub: pas de shuffle
}

bool Queue::hasNext() const { return currentIndex < mainQueue.size(); }

Tile Queue::getNext() {
    if (!hasNext()) return Tile(0, std::vector<std::vector<bool>>{{true}});
    return mainQueue[currentIndex++];
}

std::vector<Tile> Queue::peekNext(int count) {
    std::vector<Tile> out;
    for (size_t i = currentIndex; i < mainQueue.size() && out.size() < static_cast<size_t>(count); ++i) {
        out.push_back(mainQueue[i]);
    }
    return out;
}

Tile Queue::exchangeTile(int /*selectedIndex*/) {
    // Stub: renvoie une tuile par défaut
    return Tile(0, std::vector<std::vector<bool>>{{true}});
}

void Queue::returnToQueue(const Tile& tile) {
    mainQueue.push_back(tile);
}

int Queue::getRemainingCount() const {
    return static_cast<int>(mainQueue.size() - currentIndex);
}


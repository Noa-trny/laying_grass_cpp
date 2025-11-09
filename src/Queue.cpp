#include "../include/Queue.hpp"
#include "../include/Constants.hpp"
#include <random>
#include <algorithm>

Queue::Queue(int numPlayers_, const std::vector<Tile>& sourceTiles)
    : currentIndex(0), totalTiles(TILES_PER_PLAYER * numPlayers_), numPlayers(numPlayers_) {
    // Copier jusqu'a totalTiles depuis la source (ou tout si moins)
    mainQueue.clear();
    int limit = std::min(static_cast<int>(sourceTiles.size()), totalTiles);
    mainQueue.reserve(limit);
    for (int i = 0; i < limit; ++i) mainQueue.push_back(sourceTiles[i]);
    if (limit < totalTiles) {
        // si source insuffisante, completer en reutilisant des tuiles depuis le debut
        for (int i = limit; i < totalTiles && !sourceTiles.empty(); ++i) {
            mainQueue.push_back(sourceTiles[i % sourceTiles.size()]);
        }
    }
    shuffleQueue();
}

void Queue::shuffleQueue() {
    std::random_device rd; std::mt19937 g(rd());
    std::shuffle(mainQueue.begin(), mainQueue.end(), g);
}

bool Queue::hasNext() const { return currentIndex < mainQueue.size(); }

Tile Queue::getNext() {
    if (!hasNext()) return Tile(-1, std::vector<std::vector<bool>>{});
    return mainQueue[currentIndex++];
}

std::vector<Tile> Queue::peekNext(int count) const {
    std::vector<Tile> out;
    for (size_t i = currentIndex; i < mainQueue.size() && static_cast<int>(out.size()) < count; ++i) {
        out.push_back(mainQueue[i]);
    }
    return out;
}

Tile Queue::exchangeTile(int selectedIndex) {
    auto preview = peekNext(EXCHANGE_PREVIEW_SIZE);
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(preview.size())) {
        return Tile(-1, std::vector<std::vector<bool>>{});
    }
    int targetId = preview[selectedIndex].getId();
    for (size_t i = currentIndex; i < mainQueue.size(); ++i) {
        if (mainQueue[i].getId() == targetId) {
            Tile chosen = mainQueue[i];
            mainQueue.erase(mainQueue.begin() + static_cast<long long>(i));
            return chosen;
        }
    }
    return Tile(-1, std::vector<std::vector<bool>>{});
}

void Queue::returnToQueue(const Tile& tile) { mainQueue.push_back(tile); }

int Queue::getRemainingCount() const { return static_cast<int>(mainQueue.size() - currentIndex); }

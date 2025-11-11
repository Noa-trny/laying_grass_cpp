#include "../include/Queue.hpp"
#include "../include/Constants.hpp"
#include "../include/TileParser.hpp"
#include <algorithm>
#include <random>
#include <filesystem>

Queue::Queue(int numPlayers_)
    : currentIndex(0), totalTiles(TILES_PER_PLAYER * numPlayers_), numPlayers(numPlayers_) {
    initializeQueue();
}

void Queue::initializeQueue() {
    mainQueue.clear();
    exchangeQueue.clear();
    currentIndex = 0;
    
    std::vector<Tile> allTiles;
    std::vector<std::string> candidates = {
        "data/tiles.json",
        "./data/tiles.json",
        "../data/tiles.json",
        "./tiles.json",
        "tiles.json"
    };
    
    for (const auto& path : candidates) {
        try {
            allTiles = TileParser::loadTilesFromJson(path);
            break;
        } catch (...) {
            continue;
        }
    }
    
    if (allTiles.empty()) {
        allTiles.emplace_back(0, std::vector<std::vector<bool>>{{true}});
    }
    
    int tilesNeeded = TILES_PER_PLAYER * numPlayers;
    while (static_cast<int>(mainQueue.size()) < tilesNeeded && !allTiles.empty()) {
        for (const auto& tile : allTiles) {
            if (static_cast<int>(mainQueue.size()) >= tilesNeeded) break;
            mainQueue.push_back(tile);
        }
    }
    
    if (static_cast<int>(mainQueue.size()) > tilesNeeded) {
        mainQueue.resize(tilesNeeded);
    }
    
    shuffleQueue();
}

void Queue::shuffleQueue() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(mainQueue.begin(), mainQueue.end(), g);
}

bool Queue::hasNext() const { 
    return currentIndex < mainQueue.size() || !exchangeQueue.empty();
}

Tile Queue::getNext() {
    if (currentIndex < mainQueue.size()) {
        return mainQueue[currentIndex++];
    }
    
    if (!exchangeQueue.empty()) {
        Tile t = exchangeQueue.front();
        exchangeQueue.erase(exchangeQueue.begin());
        return t;
    }
    
    return Tile(0, std::vector<std::vector<bool>>{{true}});
}

std::vector<Tile> Queue::peekNext(int count) {
    std::vector<Tile> out;
    for (size_t i = currentIndex; i < mainQueue.size() && out.size() < static_cast<size_t>(count); ++i) {
        out.push_back(mainQueue[i]);
    }
    for (size_t i = 0; i < exchangeQueue.size() && out.size() < static_cast<size_t>(count); ++i) {
        out.push_back(exchangeQueue[i]);
    }
    return out;
}

Tile Queue::exchangeTile(int selectedIndex) {
    if (selectedIndex < 0 || selectedIndex >= EXCHANGE_PREVIEW_SIZE) {
        return getNext();
    }
    
    std::vector<Tile> preview = peekNext(EXCHANGE_PREVIEW_SIZE);
    if (selectedIndex >= static_cast<int>(preview.size())) {
        return getNext();
    }
    
    Tile selected = preview[selectedIndex];
    
    for (int i = 0; i < selectedIndex; ++i) {
        if (i < static_cast<int>(preview.size()) && currentIndex + i < mainQueue.size()) {
            exchangeQueue.push_back(mainQueue[currentIndex + i]);
        }
    }
    
    currentIndex += selectedIndex + 1;
    
    return selected;
}

void Queue::returnToQueue(const Tile& tile) {
    exchangeQueue.push_back(tile);
}

int Queue::getRemainingCount() const {
    return static_cast<int>(mainQueue.size() - currentIndex) + static_cast<int>(exchangeQueue.size());
}


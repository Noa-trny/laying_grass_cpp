#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Tile.hpp"
#include <vector>

class Queue {
private:
    std::vector<Tile> mainQueue;
    size_t currentIndex;
    int totalTiles; // cible théorique (peut être <= mainQueue.size())
    int numPlayers;

    void shuffleQueue();

public:
    // Construction à partir du nombre de joueurs et d'une source de tuiles (copie puis shuffle)
    Queue(int numPlayers, const std::vector<Tile>& sourceTiles);

    [[nodiscard]] bool hasNext() const;
    [[nodiscard]] Tile getNext();
    [[nodiscard]] std::vector<Tile> peekNext(int count) const;

    // Échange: choisir une tuile parmi l'aperçu des prochaines (index basé sur 0)
    Tile exchangeTile(int selectedIndex);

    void returnToQueue(const Tile& tile);

    [[nodiscard]] int getRemainingCount() const;
};

#endif

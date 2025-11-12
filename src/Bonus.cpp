#include "../include/Bonus.hpp"
#include "../include/InputHandler.hpp"
#include <iostream>
#include <algorithm>

BonusManager::BonusManager(Board* board_) : board(board_) {}

bool BonusManager::activateTileExchange(Position pos, Player& player) {
    player.addExchangeCoupon();
    std::cout << player.getName() << " a obtenu un coupon d'echange!" << std::endl;
    return true;
}

bool BonusManager::activateStone(Position pos, Player& player) {
    std::cout << player.getName() << " peut placer une pierre." << std::endl;
    while (true) {
        Position stonePos = InputHandler::getStonePosition(board->getSize());
        if (board->getCell(stonePos.row, stonePos.col) == 0 && !board->hasStone(stonePos.row, stonePos.col) && !board->isBonusSquare(stonePos.row, stonePos.col)) {
            board->placeStone(stonePos);
            std::cout << "Pierre placee." << std::endl;
            return true;
        }
        std::cout << "Position invalide pour la pierre. Reessayer." << std::endl;
    }
}

bool BonusManager::activateRobbery(Position pos, Player& fromPlayer, Player& toPlayer) {
    std::cout << toPlayer.getName() << " peut voler une tuile de " << fromPlayer.getName() << "." << std::endl;
    
    auto territory = board->getTerritory(fromPlayer.getId());
    if (territory.empty()) {
        std::cout << "Aucune tuile a voler." << std::endl;
        return false;
    }
    
    std::vector<Position> positions(territory.begin(), territory.end());
    std::cout << "Choisissez une position a voler (il y a " << positions.size() << " tuiles):" << std::endl;
    
    Position stolenPos = InputHandler::getTilePosition(board->getSize());
    
    if (territory.find(stolenPos) != territory.end() && board->getCell(stolenPos.row, stolenPos.col) == fromPlayer.getId()) {
        board->removeTerritory(fromPlayer.getId(), stolenPos);
        board->addTerritory(toPlayer.getId(), stolenPos);
        board->setCell(stolenPos.row, stolenPos.col, toPlayer.getId());
        fromPlayer.removeGrassSquares(1);
        toPlayer.addGrassSquares(1);
        std::cout << "Tuile volee avec succes." << std::endl;
        return true;
    } else {
        std::cout << "Position invalide." << std::endl;
        return false;
    }
}

void BonusManager::processBonusCapture(int row, int col, int playerId, Player& player, std::vector<Player>& allPlayers) {
    if (!board->checkBonusCapture(row, col, playerId)) return;
    
    BonusType type = board->getBonusType(row, col);
    board->claimBonusSquare(row, col, playerId);
    Position bonusPos(row, col);
    
    switch (type) {
        case BonusType::TILE_EXCHANGE:
            activateTileExchange(bonusPos, player);
            break;
        case BonusType::STONE:
            activateStone(bonusPos, player);
            break;
        case BonusType::ROBBERY: {
            std::vector<int> otherPlayers;
            for (const auto& p : allPlayers) {
                if (p.getId() != playerId) {
                    auto terr = board->getTerritory(p.getId());
                    if (!terr.empty()) {
                        otherPlayers.push_back(p.getId());
                    }
                }
            }
            if (!otherPlayers.empty()) {
                std::cout << "Cibles disponibles :" << std::endl;
                for (size_t idx = 0; idx < otherPlayers.size(); ++idx) {
                    int targetIdentifier = otherPlayers[idx];
                    auto it = std::find_if(allPlayers.begin(), allPlayers.end(), [&](const Player& candidate) {
                        return candidate.getId() == targetIdentifier;
                    });
                    if (it != allPlayers.end()) {
                        std::cout << idx + 1 << ". " << it->getName() << std::endl;
                    }
                }
                int targetId = InputHandler::getRobberyTarget(otherPlayers);
                for (auto& p : allPlayers) {
                    if (p.getId() == targetId) {
                        activateRobbery(bonusPos, p, player);
                        break;
                    }
                }
            }
            break;
        }
    }
}


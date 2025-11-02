#include "../include/Game.hpp"
#include "../include/InputHandler.hpp"
#include "../include/Display.hpp"
#include <iostream>

int main() {
    try {
        int numPlayers = InputHandler::getPlayerCount();
        
        if (numPlayers < 2 || numPlayers > 9) {
            std::cerr << "Invalid number of players. Must be between 2 and 9." << std::endl;
            return 1;
        }
        
        Game game(numPlayers);
        game.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}


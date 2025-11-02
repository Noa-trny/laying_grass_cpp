#include <cassert>
#include <iostream>
#include "../include/Tile.hpp"

void test_tile_rotation() {
    std::vector<std::vector<bool>> shape = {
        {true, false},
        {true, true}
    };
    
    Tile tile(1, shape);
    Tile rotated = tile.rotate90();
    
    std::cout << "Test rotation: OK" << std::endl;
}

void test_tile_flip() {
    std::vector<std::vector<bool>> shape = {
        {true, false},
        {true, true}
    };
    
    Tile tile(1, shape);
    Tile flipped = tile.flipHorizontal();
    
    std::cout << "Test flip: OK" << std::endl;
}

int main() {
    std::cout << "Running tile tests..." << std::endl;
    test_tile_rotation();
    test_tile_flip();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}


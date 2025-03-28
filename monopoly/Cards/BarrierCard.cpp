﻿#include "BarrierCard.hpp"
#include "Tiles/Tile.hpp"
#include "InputManager.hpp"

void BarrierCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) {
    std::cout << curPlayer->getName() << " used Barrier Card! Select a tile to place a barrier." << std::endl;

    std::vector<std::shared_ptr<Tile>> tiles = Board::getInstance()->getTileList();

    //// Display tiles
    // for (size_t i = 1; i < tiles.size(); i++) {
    //     std::cout << i << ". " << tiles[i]->getName() << (tiles[i]->isBlocked() ? " [Blocked]" : "") << std::endl;
    // }

    // Select a tile
    std::cout << "Enter the tile number to put the barrier (0 to cancel): ";
    int choice;
    while (true) {
        try {
            std::string input;
            std::getline(std::cin, input);
            choice = std::stoi(input);
            std::cout << choice << std::endl;
            if (choice < 1 || choice > static_cast<int>(tiles.size())) {
                throw std::invalid_argument("Invalid input");
            }
            break;

        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a valid tile number.\n";
        }
    }

    if (choice == 0) {
        return;
    }

    std::shared_ptr<Tile> targetTile = tiles[choice];
    targetTile->setBlock(true);
    std::cout << "A barrier has been placed on " << targetTile->getName() << "!\n";
}
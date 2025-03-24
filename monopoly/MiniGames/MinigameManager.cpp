#include "MiniGameManager.hpp"
#include "Game/Board.hpp"
#include "Game/Player.hpp"
#include "DragonGateGame.hpp"
#include "HorseRacing.hpp"
#include "InputManager.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

void MiniGameManager::startMiniGame(std::shared_ptr<Player> player) {
    // Choose a mini-game to play
    Board::getInstance()->clearScreen();
    std::cout << "========================\n"
              << "      MiniGame Menu     \n"
              << "========================\n"
              << "1. Dragon Gate Game     \n"
              << "2. Horse Racing Game    \n"
              << "3. Return               \n"
              << "========================\n"
              << "Please choose (1~3): ";

    int choice = 0;
    while (!(std::cin >> choice) || (choice < 1 || choice > 3)) {
        std::cout << "Please enter 1-3: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    // Start the selected mini-game
    switch (choice) {
    case 1:
    {
        DragonGateGame drangonGateGame;
        long long bet = setBetAmount(player);
        long long reward = drangonGateGame.playGame(bet);
        if (reward < 0) {
            player->deductMoney(-reward);
        } else {
            player->addMoney(reward);
        }
        break;
    }
    case 2:
    {
        HorseRacing horseRacing;
        long long bet = setBetAmount(player);
        long long reward = horseRacing.playGame(bet);
        if (reward < 0) {
            player->deductMoney(-reward); 
        } else {
            player->addMoney(reward);
        }
        break;
    }
    case 3:
    default:
        std::cout << "Returning to main program...\n";
        break;
    }

    Board::getInstance()->drawBoard();
}

void MiniGameManager::endMiniGame(std::shared_ptr<Player> player) {
    std::cout << "Ending mini-game for player: " << player->getName() << std::endl;
    // Implement mini-game end logic here
}

void MiniGameManager::listMiniGames() {
    std::cout << "Available mini-games: \n"
              << "1) Dragon Gate Game\n"
              << "2) Horse Racing Game\n"
              << "...\n";
    // You can list more mini-games here
}

long long MiniGameManager::setBetAmount(std::shared_ptr<Player> player) {
    std::cout << "\nEnter your bet amount (you currently have " << player->getMoney() << "): ";
    long long bet = 0;
    while (!(std::cin >> bet) || bet <= 0 || bet > player->getMoney()) {
        std::cout << "Please enter a valid bet amount (1 ~ " << player->getMoney() << "): ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    Board::getInstance()->clearScreen();
    return bet;
}
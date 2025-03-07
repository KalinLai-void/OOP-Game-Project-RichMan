#include "MiniGameManager.hpp"
#include "Board.hpp"
#include "InputManager.hpp"
#include "Player.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

void delayMilliseconds(int ms);
// -----------------------------------------
// Embedded classes for "DragonGateGame" and "HorseRacing"
// -----------------------------------------
class DragonGateGame {
public:
    // Returns the amount of coins won (could be 0 or a positive value)
    long long playGame(long long betAmount) {
        std::cout << "\nStarting Dragon Gate Game!" << std::endl;

        long long totalReward = 0;
        int firstCard = rand() % 13 + 1;
        int secondCard = rand() % 13 + 1;
        if (firstCard > secondCard)
            std::swap(firstCard, secondCard);

        std::cout << "First card: " << firstCard << "  Second card: " << secondCard << std::endl;

        int choice;
        if (firstCard == secondCard) {
            std::cout << "Both cards are the same, guess (1: Higher, 2: Lower): ";
            while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
                std::cout << "Please enter 1 or 2: ";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        } else {
            std::cout << "Guess if the third card is in range (1: Yes, 2: No): ";
            while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
                std::cout << "Please enter 1 or 2: ";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        }

        int thirdCard = rand() % 13 + 1;
        std::cout << "Third card: " << thirdCard << std::endl;

        bool guessCorrect = false;
        bool hitPillar = (thirdCard == firstCard || thirdCard == secondCard);
        if (firstCard == secondCard) {
            // Guess "Higher" or "Lower"
            if ((thirdCard > firstCard && choice == 1) || (thirdCard < firstCard && choice == 2)) {
                guessCorrect = true;
            }
        } else {
            bool inRange = (thirdCard > firstCard && thirdCard < secondCard);
            if ((inRange && choice == 1) || (!inRange && choice == 2)) {
                guessCorrect = true;
            }
        }

        if (hitPillar) {
            std::cout << "Oh no! The third card is equal to one of the first two cards. You lose double!" << std::endl;
            totalReward = -2 * betAmount;
        } else if (guessCorrect) {
            std::cout << "Congratulations! You won 1000 coins!" << std::endl;
            totalReward = betAmount;
        } else {
            std::cout << "Sorry, you lost!" << std::endl;
            totalReward = -betAmount;
        }

        std::cout << "\nPress any key to continue...";
        std::cin.ignore();
        InputManager::getKey();
        return totalReward; // Default return value
    }
};

class HorseRacing {
public:
    // Takes the bet amount as input, returns positive value for winnings, negative for losses
    long long playGame(long long betAmount) {
        std::cout << "Starting Horse Racing Game!" << std::endl;

        const std::vector<std::string> colors = {"\033[31m", "\033[32m", "\033[33m", "\033[34m"}; // Red, Green, Yellow, Blue
        const std::string resetColor = "\033[0m";
        std::vector<std::string> horses = {"Deep Impact", "Orfevre", "Kitasan Black", "Gold Ship"};
        std::vector<int> odds = {2, 3, 5, 10};
        long long totalReward = 0;
        int finishLine = 30;

        std::cout << "Odds table:" << std::endl;
        for (size_t i = 0; i < horses.size(); i++)
            std::cout << colors[i] << i + 1 << ". " << horses[i] << " (Odds: " << odds[i] << "x)" << resetColor << std::endl;

        std::cout << "Choose a horse (enter 1-4): ";
        int choice;
        while (!(std::cin >> choice) || choice < 1 || choice > 4) {
            std::cout << "Please enter a number between 1-4: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "The race begins!" << std::endl;
        std::vector<int> speeds(4, 0);
        std::vector<int> ranks(4, -1);
        bool raceOver = false;
        int rankCounter = 1;
        Board::getInstance()->clearScreen();
        while (!raceOver) {
            for (int i = 0; i < 4; i++) {
                if (ranks[i] == -1) {            // Only move horses that haven't finished
                    speeds[i] += rand() % 3 + 1; // 1~3
                    if (speeds[i] >= finishLine) {
                        ranks[i] = rankCounter++;
                    }
                }
            }
            std::cout << "\r";
            std::cout << "Start Line:\t|" << std::string(finishLine, '-') << "| Finish Line" << std::endl;
            for (int i = 0; i < 4; i++) {
                if (speeds[i] >= finishLine) {
                    std::cout << colors[i] << horses[i] << ":\t|" << std::string(finishLine, '-') << "|" << resetColor << " Finish! (Rank: " << ranks[i] << ")"
                              << std::endl;
                } else {
                    std::cout << colors[i] << horses[i] << ":\t|" << std::string(speeds[i], '-') << "|" << resetColor << std::endl;
                }
            }
            if (rankCounter > 4) {
                raceOver = true;
            } else {
                delayMilliseconds(200);
                Board::getInstance()->clearScreen();
            }
        }

        // Find the winning horse
        int winner = std::distance(ranks.begin(), std::find(ranks.begin(), ranks.end(), 1));
        std::cout << "The winning horse is: " << colors[winner] << horses[winner] << resetColor << "!" << std::endl;

        if (choice - 1 == winner) {
            totalReward = betAmount * odds[choice - 1];
            std::cout << "Congratulations! You won " << totalReward << " coins!" << std::endl;
        } else {
            std::cout << "Sorry, you lost." << std::endl;
            totalReward = -betAmount;
        }

        std::cout << "\nPress any key to continue...";
        std::cin.ignore();
        InputManager::getKey();
        return totalReward; // Default return value
    }
};

// -----------------------------------------
// Implementation of MiniGameManager functions
// -----------------------------------------

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

    // Get the bet amount from the player
    std::cout << "\nEnter your bet amount (you currently have " << player->getMoney() << "): ";
    long long bet = 0;
    while (!(std::cin >> bet) || bet <= 0 || bet > player->getMoney()) {
        std::cout << "Please enter a valid bet amount (1 ~ " << player->getMoney() << "): ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    Board::getInstance()->clearScreen();

    // Start the selected mini-game
    switch (choice) {
    case 1:
    {
        DragonGateGame dragonGateGame;
        long long reward = dragonGateGame.playGame(bet);
        if (reward > 0) {
            player->addMoney(reward);
        }
        break;
    }
    case 2:
    {
        HorseRacing horseRacing;
        long long result = horseRacing.playGame(bet);
        if (result < 0) {
            player->deductMoney(result);
        } else {
            player->addMoney(result);
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

void delayMilliseconds(int ms) {
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(ms)) {
        // Busy-waiting loop (not CPU-efficient but avoids thread sleep issues)
    }
}
// MiniGameManager.cpp

#include "MiniGameManager.hpp"
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
void clearScreen();

// -----------------------------------------
// Embedded classes for "CardGame" and "HorseRacing"
// -----------------------------------------
class CardGame {
public:
    // Returns the amount of coins won (could be 0 or a positive value)
    long long playGame() {
        std::cout << "\nStarting Card Game!" << std::endl;

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

        if (guessCorrect) {
            std::cout << "Congratulations! You won 1000 coins!" << std::endl;
            totalReward = 1000;
        } else {
            std::cout << "Sorry, you lost!" << std::endl;
        }

        return totalReward;
    }
};

class HorseRacing {
public:
    // Takes the bet amount as input, returns positive value for winnings, negative for losses
    long long playGame(long long betAmount) {
        std::cout << "\nStarting Horse Racing Game!" << std::endl;

        const std::vector<std::string> colors = {"\033[31m", "\033[32m", "\033[33m", "\033[34m"}; // Red, Green, Yellow, Blue
        const std::string resetColor = "\033[0m";
        std::vector<std::string> horses = {"Deep Impact", "Orfevre", "Kitasan Black", "Gold Ship"};
        std::vector<int> odds = {2, 3, 5, 10};

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

        // Simple simulation of 30 steps with different colors for each horse
        for (int step = 0; step < 30; step++) {
            for (int i = 0; i < 4; i++) {
                speeds[i] += rand() % 3 + 1; // 1~3
            }
            // Output current progress (very simple)
            std::cout << "\r";
            for (int i = 0; i < 4; i++) {
                std::cout << colors[i] << horses[i] << ": " << std::string(speeds[i], '-') << "|" << resetColor << "\n";
            }
            delayMilliseconds(200);
            clearScreen();
        }

        // Find the winning horse
        int winner = std::max_element(speeds.begin(), speeds.end()) - speeds.begin();
        std::cout << "The winning horse is: " << horses[winner] << "!" << std::endl;

        if (choice - 1 == winner) {
            long long winnings = betAmount * odds[choice - 1];
            std::cout << "Congratulations! You won " << winnings << " coins!" << std::endl;
            return winnings; // Return positive value
        } else {
            std::cout << "Sorry, you lost." << std::endl;
            return -betAmount; // Return negative value
        }
    }
};

// -----------------------------------------
// Implementation of MiniGameManager functions
// -----------------------------------------

void MiniGameManager::startMiniGame(std::shared_ptr<Player> player) {
    // You can write the process of "choosing which mini-game to play" here
    // Or you can directly call a specific game, depending on your needs
    std::cout << "\n========================\n"
              << "MiniGame Menu\n"
              << "1. Card Game\n"
              << "2. Horse Racing Game\n"
              << "3. Return\n"
              << "========================\n"
              << "Please choose (1~3): ";

    int choice = 0;
    while (!(std::cin >> choice) || (choice < 1 || choice > 3)) {
        std::cout << "Please enter 1-3: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    switch (choice) {
    case 1:
    {
        CardGame cardGame;
        long long reward = cardGame.playGame();
        if (reward > 0) {
            player->adjustMoney(reward);
        }
        break;
    }
    case 2:
    {
        HorseRacing horseRacing;
        std::cout << "Enter your bet amount (you currently have " << player->getMoney() << "): ";
        long long bet = 0;
        while (!(std::cin >> bet) || bet <= 0 || bet > player->getMoney()) {
            std::cout << "Please enter a valid bet amount (1 ~ " << player->getMoney() << "): ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        long long result = horseRacing.playGame(bet);
        if (result < 0) {
            player->adjustMoney(-result); // result is negative, deduct -result
        } else {
            player->adjustMoney(result);
        }
        break;
    }
    case 3:
    default:
        std::cout << "Returning to main program...\n";
        break;
    }
}

void MiniGameManager::endMiniGame(std::shared_ptr<Player> player) {
    std::cout << "Ending mini-game for player: " << player->getName() << std::endl;
    // Implement mini-game end logic here
}

void MiniGameManager::listMiniGames() const {
    std::cout << "Available mini-games: \n"
              << "1) Card Game\n"
              << "2) Horse Racing Game\n"
              << "...\n";
    // You can list more mini-games here
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H"; // ANSI Escape Code to clear the screen and move the cursor to the top-left corner
#endif
}

void delayMilliseconds(int ms) {
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(ms)) {
        // Busy-waiting loop (not CPU-efficient but avoids thread sleep issues)
    }
}

// -----------------------------------------
// Main function as the entry point of the program
// -----------------------------------------
int main() {
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    // Create mini-game manager
    std::shared_ptr<MiniGameManager> miniGameManager = std::make_shared<MiniGameManager>();

    // Create player
    std::shared_ptr<Player> player = std::make_shared<Player>("TestPlayer", "T", 5000ll);

    // Main loop: player can repeatedly enter different mini-games until they choose to exit
    while (true) {
        std::cout << "\n========== Main Menu ==========\n"
                  << "Player: " << player->getName() << " (Balance: " << player->getMoney() << ")\n"
                  << "1) Start Mini-Game\n"
                  << "2) End Mini-Game (Test)\n"
                  << "3) List Available Mini-Games\n"
                  << "4) Exit Program\n"
                  << "==========================\n"
                  << "Please choose (1~4): ";

        int c;
        while (!(std::cin >> c) || c < 1 || c > 4) {
            std::cout << "Please enter 1-4: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        if (c == 1) {
            miniGameManager->startMiniGame(player);
        } else if (c == 2) {
            miniGameManager->endMiniGame(player);
        } else if (c == 3) {
            miniGameManager->listMiniGames();
        } else {
            // Exit
            std::cout << "Thank you for playing!\n";
            break;
        }
    }

    return 0;
}

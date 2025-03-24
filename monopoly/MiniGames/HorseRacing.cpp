#include "HorseRacing.hpp"
#include "Game/Board.hpp"
#include "Game/Player.hpp"
#include "Utils/Utils.hpp"
#include "InputManager.hpp"

long long HorseRacing::playGame(long long betAmount) {
    // Random engine and distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> speedDist(1, 3);

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
                speeds[i] += speedDist(gen); // 1~3
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
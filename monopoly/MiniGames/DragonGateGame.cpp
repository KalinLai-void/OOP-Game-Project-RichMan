#include "DragonGateGame.hpp"
#include "Utils/Utils.hpp"
#include "InputManager.hpp"

long long DragonGateGame::playGame(long long betAmount) {
    // Random engine and distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> cardDist(1, 13);

    std::cout << "\nStarting Dragon Gate Game!" << std::endl;

    long long totalReward = 0;
    int firstCard = cardDist(gen);
    int secondCard = cardDist(gen);
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

    int thirdCard = cardDist(gen);
    std::cout << "Third card: " << thirdCard << std::endl;

    bool guessCorrect = false;
    bool hitPillar = (thirdCard == firstCard || thirdCard == secondCard);
    bool hitTwoPillars = (firstCard == secondCard && hitPillar);
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
    if (hitTwoPillars) {
        std::cout << "Disaster strikes! The third card matches the first two, and you lose triple your bet!" << std::endl;
        std::cout << "You lose " << 3 * betAmount << " coins!" << std::endl;
        totalReward = -3 * betAmount;
    } else if (hitPillar) {
        std::cout << "Oh no! The third card is equal to one of the first two cards. You lose double!" << std::endl;
        std::cout << "You lose " << 2 * betAmount << " coins!" << std::endl;
        totalReward = -2 * betAmount;
    } else if (guessCorrect) {
        std::cout << "Congratulations! You won " << betAmount << " coins!" << std::endl;
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
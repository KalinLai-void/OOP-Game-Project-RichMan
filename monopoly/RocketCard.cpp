#include "RocketCard.hpp"
#include <iostream>

void RocketCard::useEffect(std::shared_ptr<Player> player) {
    std::string str;
    int stepsForward;
    size_t pos;
    while (true) {
        std::cout << "Choose the number you roll (1-32): ";
        std::getline(std::cin, str);

        stepsForward = std::stoi(str, &pos);

        if (pos == str.size() && stepsForward >= 1 && stepsForward <= 32) {
            player->setPosition(stepsForward);
            return;
        }
        std::cout << "Invalid input. Please enter a number between 1 and 31." << std::endl;
    }
}
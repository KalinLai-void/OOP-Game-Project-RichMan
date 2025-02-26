#include "DiceControlCard.hpp"
#include "InputManager.hpp"
#include <iostream>

void DiceControlCard::useEffect(std::shared_ptr<Player> player) {
    std::string str;
    int chosenRoll;
    size_t pos;
    while (true) {
        std::cout << "Choose the number you roll (1-12): ";
        std::getline(std::cin, str);

        chosenRoll = std::stoi(str, &pos);

        if (pos == str.size() && chosenRoll >= 1 && chosenRoll <= 12) {
            player->setPosition(chosenRoll);
            return;
        }
        std::cout << "Invalid input. Please enter a number between 1 and 12." << std::endl;

    }
}
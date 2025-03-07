#include "DiceCard.hpp"
#include "InputManager.hpp"
#include <iostream>
#include <vector>

void DiceCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) {
    std::cout << "Choose the number you roll (2-12): ";
    int chosenRoll;
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        try {
            size_t pos;
            chosenRoll = std::stoi(input, &pos);
            if (pos != input.size() || chosenRoll < 2 || chosenRoll > 12) {
                throw std::invalid_argument("Invalid input");
            }
            curPlayer->setDiceControl(chosenRoll);
            return;
        } catch (const std::exception&) {
            std::cout << "Invalid input. Please choose the number you roll (2-12):";
        }
    }
}
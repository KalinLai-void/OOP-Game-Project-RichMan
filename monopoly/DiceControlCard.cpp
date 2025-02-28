#include "DiceControlCard.hpp"
#include "InputManager.hpp"
#include <iostream>
#include <vector>

void DiceControlCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer, Board& board) {
    std::cout << "Choose the number you roll (1-6): ";
    int chosenRoll;
    while (true) {
        std::string input;
        std::cin >> input;
        try {
            size_t pos;
            chosenRoll = std::stoi(input, &pos);
            if (pos!=input.size() || chosenRoll < 1 || chosenRoll > 6) {
                throw std::invalid_argument("Invalid input");
            }
            curPlayer->setDiceControl(chosenRoll);
            return;
        } catch (const std::exception&){
            std::cout << "Invalid input. Please choose the number you roll (1-6):";
        }
    }
}
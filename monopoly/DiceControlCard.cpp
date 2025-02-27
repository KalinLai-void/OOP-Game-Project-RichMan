#include "DiceControlCard.hpp"
#include "InputManager.hpp"
#include <iostream>
#include <vector>

void DiceControlCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer, Board& board) {
    std::cout << "Choose the number you roll (1-6): ";
    int chosenRoll;
    while (true) {
        std::string input;
        
        std::getline(std::cin, input);
        try {
            size_t pos;
            chosenRoll = std::stoi(input, &pos);
            if (chosenRoll >= 1 && chosenRoll <= 6) {
                curPlayer->setDiceControl(chosenRoll);
                return;
            } else {
                std::cout << "Invalid input. Please enter a number between 1 and 6." << std::endl;
            }
        } catch (const std::exception&){
            std::cout << "Invalid input. Please enter a number between 1 and 6." << std::endl;
        }
    }
}
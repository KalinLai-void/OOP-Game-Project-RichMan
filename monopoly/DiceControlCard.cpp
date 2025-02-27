#include "DiceControlCard.hpp"
#include "InputManager.hpp"
#include <iostream>
#include <vector>

void DiceControlCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> player, Board& board) {
    int chosenRoll;
    while (true) {
        std::cout << "Choose the number you roll (1-12): ";
        std::cin >> chosenRoll;

        if (chosenRoll >= 1 && chosenRoll <= 12) {
            int newPos = (player->getPosition() + chosenRoll) % board.getSize();
            player->setPosition(newPos);
            board.drawBoard(players);
            return;
        }
        std::cout << "Invalid input. Please enter a number between 1 and 12." << std::endl;

    }
}